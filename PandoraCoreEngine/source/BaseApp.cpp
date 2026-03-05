#include "BaseApp.h"
#include "ResourceManager.h"

HRESULT 
BaseApp::awake() {
    HRESULT hr = S_OK;
    // 1. Iniciar grafo de escena
    m_sceneGraph.init();
    MESSAGE("Main", "Awake", "Application awake successfully.");
    return hr;
}

int 
BaseApp::run(HINSTANCE hInst, int nCmdShow) {
    // 2. Iniciar ventana
    if (FAILED(m_window.init(hInst, nCmdShow, WndProc))) return 0;
    // 3. Ciclo Awake -> Init
    if (FAILED(awake())) return 0;
    if (FAILED(init())) return 0;

    m_gui.init(m_window, m_device, m_deviceContext);

    MSG msg = {};
    LARGE_INTEGER freq, prev;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            LARGE_INTEGER curr;
            QueryPerformanceCounter(&curr);
            float deltaTime = static_cast<float>
              (curr.QuadPart - prev.QuadPart) / freq.QuadPart;
            prev = curr;
            update(deltaTime);
            render();
        }
    }
    return (int)msg.wParam;
}

HRESULT 
BaseApp::init() {
    HRESULT hr = S_OK;

    // 4. Recursos básicos de D3D11
    hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window);
    hr = m_renderTargetView.init(m_device, m_backBuffer, 
      DXGI_FORMAT_R8G8B8A8_UNORM);

    // 5. Configuración de Profundidad (Depth)
    UINT sampleCount = 4;
    UINT quality = 0;
    m_device.m_device->CheckMultisampleQualityLevels(
      DXGI_FORMAT_D24_UNORM_S8_UINT, sampleCount, &quality);
    if (quality > 0) quality = quality - 1;

    hr = m_depthStencil.init(m_device,
                             m_window.m_width, 
                             m_window.m_height, 
                             DXGI_FORMAT_D24_UNORM_S8_UINT, 
                             D3D11_BIND_DEPTH_STENCIL, 
                             sampleCount, quality);
    hr = m_depthStencilView.init(m_device, 
                                 m_depthStencil, 
                                 DXGI_FORMAT_D24_UNORM_S8_UINT);
    hr = m_viewport.init(m_window);

    // 6. Carga de Skybox
    std::array<std::string, 6> faces = {
        "Skybox/cubemap_0.png", "Skybox/cubemap_1.png", "Skybox/cubemap_2.png",
        "Skybox/cubemap_3.png", "Skybox/cubemap_4.png", "Skybox/cubemap_5.png"
    };
    m_skyboxTex.CreateCubemap(m_device, m_deviceContext, faces, true);

    // 7. Configuración del Actor (VALORES M4A1)
    m_modelActor = EU::MakeShared<Actor>(m_device);
    if (!m_modelActor.isNull()) {
        m_modelResource = new Model3D("Assets/M4A1.fbx", ModelType::FBX);
        
        std::vector<MeshComponent> meshes = m_modelResource->GetMeshes();
        m_modelAlbedo.init(m_device, "Assets/Text", ExtensionType::PNG);

        std::vector<Texture> textures;
        textures.push_back(m_modelAlbedo);

        m_modelActor->setMesh(m_device, meshes);
        m_modelActor->setTextures(textures);
        m_modelActor->setName("CyberGun"); 
        
        m_modelActor->getComponent<Transform>()->setTransform(
            EU::Vector3(-3.80f, 2.88f, 36.00f), 
            EU::Vector3(-90.0f, 1.60f, -2.00f), 
            EU::Vector3(0.80f, 0.80f, 0.80f)
        );
        m_actors.push_back(m_modelActor);
    }

    // 8. Registro en el Grafo
    for (auto& actor : m_actors) {
        m_sceneGraph.addEntity(actor.get());
    }

    // 9. Pipeline de Shaders
    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    m_shaderProgram.init(m_device, "PandoraCoreEngine.fx", Layout);

    // 10. Buffers Constantes y Cámara
    m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges));
    m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize));
    m_camera.setLens(XM_PIDIV4, m_window.m_width / 
      (float)m_window.m_height, 0.01f, 100.0f);
    m_camera.setPosition(0.0f, 3.0f, -6.0f);
    
    return S_OK;
}

void 
BaseApp::update(float deltaTime) {
    m_gui.update(m_viewport, m_window);

    // 11. Debug SRVs Skybox
    if (!m_faceDebugSRV[0]) {
        for (UINT i = 0; i < 6; ++i) {
            m_faceDebugSRV[i] = m_skyboxTex.CreateCubemapFaceSRV(
                m_device.m_device, m_skyboxTex.m_texture,
                DXGI_FORMAT_R8G8B8A8_UNORM, i, 1);
        }
    }

    // 12. UI Panel Debug
    ImGui::Begin("Debug");
    ImGui::Text("Cubemap Faces:");
    for (int i = 0; i < 6; ++i) {
        if (m_faceDebugSRV[i]) 
          ImGui::Image((ImTextureID)m_faceDebugSRV[i], ImVec2(100, 100));
        if ((i % 3) != 2) ImGui::SameLine();
    }
    ImGui::End();

    // 13. UI Panel Cubemap
    ImGui::Begin("Cubemap");
    ImGui::Text("Skybox Cubemap");
    if (m_faceDebugSRV[0]) {
        ImGui::Image((ImTextureID)m_faceDebugSRV[0], ImVec2(256, 256));
    }
    ImGui::End();

    // 14. Inspector y Outliner
    if (m_gui.selectedActorIndex >= 0 && 
        m_gui.selectedActorIndex < m_actors.size()) {
        m_gui.inspectorGeneral(m_actors[m_gui.selectedActorIndex]);
        m_gui.editTransform(m_camera.getView(), 
          m_camera.getProj(), m_actors[m_gui.selectedActorIndex]);
    }
    m_gui.outliner(m_actors);

    // 15. Matrices
    m_camera.updateViewMatrix();
    cbNeverChanges.mView = XMMatrixTranspose(m_camera.getView());
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_camera.getProj());
    m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr,
      &cbNeverChanges, 0, 0);
    m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, 
      &cbChangesOnResize, 0, 0);

    m_sceneGraph.update(deltaTime, m_deviceContext);
}

void 
BaseApp::render() {
    float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);
    m_viewport.render(m_deviceContext);
    m_depthStencilView.render(m_deviceContext);
    
    m_shaderProgram.render(m_deviceContext);
    m_cbNeverChanges.render(m_deviceContext, 0, 1);
    m_cbChangeOnResize.render(m_deviceContext, 1, 1);
    
    m_sceneGraph.render(m_deviceContext);
    m_gui.render();
    m_swapChain.present();
}

void 
BaseApp::destroy() {
    if (m_deviceContext.m_deviceContext) 
      m_deviceContext.m_deviceContext->ClearState();
    
    for (int i = 0; i < 6; ++i) {
        if (m_faceDebugSRV[i]) { 
          m_faceDebugSRV[i]->Release(); 
          m_faceDebugSRV[i] = nullptr; 
        }
    }

    if (m_modelResource) {
        delete m_modelResource;
        m_modelResource = nullptr;
    }

    m_sceneGraph.destroy();
    m_cbNeverChanges.destroy();
    m_cbChangeOnResize.destroy();
    m_shaderProgram.destroy();
    m_depthStencil.destroy();
    m_depthStencilView.destroy();
    m_renderTargetView.destroy();
    m_swapChain.destroy();
    m_backBuffer.destroy();
    m_skyboxTex.destroy();
    m_gui.destroy();
    m_deviceContext.destroy();
    m_device.destroy();
}

LRESULT 
BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) 
      return true;
    switch (message) {
        case WM_CREATE: {
            CREATESTRUCT* p = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)p->lpCreateParams);
        } return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        } return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}