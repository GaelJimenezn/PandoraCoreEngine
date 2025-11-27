#include "BaseApp.h"
#include "ResourceManager.h"

// Constructor implementation
BaseApp::BaseApp(HINSTANCE hInst, int nCmdShow) : m_model(nullptr) {
    // Initialization can be done here or in init()
}

// Destructor implementation
BaseApp::~BaseApp() {
    destroy();
}

int 
BaseApp::run(HINSTANCE hInst, int nCmdShow) {
  if (FAILED(m_window.init(hInst, nCmdShow, WndProc))) {
    return 0;
  }
  if (FAILED(init()))
    return 0;
  // Main message loop
  MSG msg = {};
  LARGE_INTEGER freq, prev;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&prev);
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      LARGE_INTEGER curr;
      QueryPerformanceCounter(&curr);
      float deltaTime = static_cast<float>(curr.QuadPart - prev.QuadPart) / freq.QuadPart;
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

    // Crear swapchain
    hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize SwapChain.");
        return hr;
    }

    // Crear render target view
    hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize RenderTargetView.");
        return hr;
    }

    // Crear textura de depth stencil
    hr = m_depthStencil.init(m_device,
        m_window.m_width,
        m_window.m_height,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        D3D11_BIND_DEPTH_STENCIL,
        1,
        0);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize DepthStencil.");
        return hr;
    }

    // Crear el depth stencil view
    hr = m_depthStencilView.init(m_device,
        m_depthStencil,
        DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize DepthStencilView.");
        return hr;
    }

    // Crear el m_viewport
    hr = m_viewport.init(m_window);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize Viewport.");
        return hr;
    }

    // Define the input layout
    // NOTA: Agregamos NORMAL porque Model3D extrae normales
    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
    D3D11_INPUT_ELEMENT_DESC position;
    position.SemanticName = "POSITION";
    position.SemanticIndex = 0;
    position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    position.InputSlot = 0;
    position.AlignedByteOffset = 0;
    position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    position.InstanceDataStepRate = 0;
    Layout.push_back(position);

    D3D11_INPUT_ELEMENT_DESC texcoord;
    texcoord.SemanticName = "TEXCOORD";
    texcoord.SemanticIndex = 0;
    texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
    texcoord.InputSlot = 0;
    texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    texcoord.InstanceDataStepRate = 0;
    Layout.push_back(texcoord);

    D3D11_INPUT_ELEMENT_DESC normal;
    normal.SemanticName = "NORMAL";
    normal.SemanticIndex = 0;
    normal.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    normal.InputSlot = 0;
    normal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    normal.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    normal.InstanceDataStepRate = 0;
    Layout.push_back(normal);

    // Create the Shader Program
    hr = m_shaderProgram.init(m_device, "WildvineEngine.fx", Layout);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize ShaderProgram.");
        return hr;
    }

    // --- CARGA DEL MODELO ---
    
    m_model = new Model3D("nissan", ModelType::FBX);
   // m_model = new Model3D("nissan_gt-r", ModelType::OBJ);

    // Create buffers for ALL meshes found in the model
    const std::vector<MeshComponent>& meshes = m_model->GetMeshes();
    if (meshes.empty()) {
        ERROR(L"BaseApp", L"init", L"Model loaded but returned no meshes!");
    }

    for (const auto& mesh : meshes) {
        // Create vertex buffer
        Buffer vb;
        hr = vb.init(m_device, mesh, D3D11_BIND_VERTEX_BUFFER);
        if (FAILED(hr)) {
            ERROR(L"BaseApp", L"init", L"Failed to initialize VertexBuffer.");
            return hr;
        }
        m_vertexBuffers.push_back(vb);

        // Create index buffer
        Buffer ib;
        hr = ib.init(m_device, mesh, D3D11_BIND_INDEX_BUFFER);
        if (FAILED(hr)) {
            ERROR(L"BaseApp", L"init", L"Failed to initialize IndexBuffer.");
            return hr;
        }
        m_indexBuffers.push_back(ib);
    }

    // Create the constant buffers
    hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges));
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize NeverChanges Buffer.");
        return hr;
    }

    hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize));
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize ChangeOnResize Buffer.");
        return hr;
    }

    hr = m_cbChangesEveryFrame.init(m_device, sizeof(CBChangesEveryFrame));
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize ChangesEveryFrame Buffer.");
        return hr;
    }

    // Load the Texture (Shared for both models as requested)
    hr = m_modelTexture.init(m_device, "nissan_bake", ExtensionType::PNG);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize texture.");
        // Non-critical, continue
    }

    // Create the sample state
    hr = m_samplerState.init(m_device);
    if (FAILED(hr)) {
        ERROR(L"BaseApp", L"init", L"Failed to initialize SamplerState.");
        return hr;
    }

    // Initialize matrices
    m_World = XMMatrixIdentity();

    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_View = XMMatrixLookAtLH(Eye, At, Up);

    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

    return S_OK;
}

void BaseApp::update(float deltaTime)
{
    // Update logic
    
    // Update constant buffers that rarely change
    m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
    m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    // Modify the color
    m_vMeshColor.x = 1.0f;
    m_vMeshColor.y = 1.0f;
    m_vMeshColor.z = 1.0f;
    m_vMeshColor.w = 1.0f;

    // Transform logic
    XMMATRIX scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(-0.60f, 3.0f * deltaTime, -0.20f); // Added time for rotation
    XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

    m_World = scaleMatrix * rotationMatrix * translationMatrix;
    
    cb.mWorld = XMMatrixTranspose(m_World);
    cb.vMeshColor = m_vMeshColor;
    m_cbChangesEveryFrame.update(m_deviceContext, nullptr, 0, nullptr, &cb, 0, 0);
}

void 
BaseApp::render() {
    // Set Render Target View
    float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

    // Set Viewport
    m_viewport.render(m_deviceContext);

    // Set depth stencil view
    m_depthStencilView.render(m_deviceContext);

    // Set shader program
    m_shaderProgram.render(m_deviceContext);

    // Set constant buffers
    m_cbNeverChanges.render(m_deviceContext, 0, 1);
    m_cbChangeOnResize.render(m_deviceContext, 1, 1);
    m_cbChangesEveryFrame.render(m_deviceContext, 2, 1);

    // Set texture and sampler
    m_modelTexture.render(m_deviceContext, 0, 1);
    m_samplerState.render(m_deviceContext, 0, 1);

    m_deviceContext.iaSetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Render loop for all meshes
    const std::vector<MeshComponent>& meshes = m_model->GetMeshes();
    for (size_t i = 0; i < meshes.size(); ++i) {
        // Safety check to ensure we have buffers for this mesh
        if (i < m_vertexBuffers.size() && i < m_indexBuffers.size()) {
            m_vertexBuffers[i].render(m_deviceContext, 0, 1);
            m_indexBuffers[i].render(m_deviceContext, 0, 1, false, DXGI_FORMAT_R32_UINT);
            
            m_deviceContext.drawIndexed(meshes[i].m_numIndex, 0, 0);
        }
    }

    // Present our back buffer to our front buffer
    m_swapChain.present();
}

void 
BaseApp::destroy() {
    if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();

    if (m_model) {
        m_model->unload();
        delete m_model;
        m_model = nullptr;
    }

    // Destroy buffers in vectors
    for(auto& vb : m_vertexBuffers) vb.destroy();
    for(auto& ib : m_indexBuffers) ib.destroy();
    m_vertexBuffers.clear();
    m_indexBuffers.clear();

    m_samplerState.destroy();
    m_modelTexture.destroy();

    m_cbNeverChanges.destroy();
    m_cbChangeOnResize.destroy();
    m_cbChangesEveryFrame.destroy();
    m_shaderProgram.destroy();
    m_depthStencil.destroy();
    m_depthStencilView.destroy();
    m_renderTargetView.destroy();
    m_swapChain.destroy();
    m_backBuffer.destroy();
    m_deviceContext.destroy();
    m_device.destroy();
}

LRESULT CALLBACK
BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message)
  {
  case WM_CREATE:
  {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);
  }
  return 0;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}