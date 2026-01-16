#include "BaseApp.h"
#include "ResourceManager.h"

HRESULT
BaseApp::awake() {
  HRESULT hr = S_OK;

  //inicializacion de dlls y elementos externos del motor 

  //log success message
  MESSAGE("Main", "Awake", "Application Awake successfuly");
  return hr;
}

int
BaseApp::run(HINSTANCE hInst, int nCmdShow) {
  //1) initialize window
  if (FAILED(m_window.init(hInst, nCmdShow, WndProc))) {
    ERROR("Main", "Run", "Failed to initialize window");
    return 0;
  }
  //2) Awake application
  if(FAILED(awake())) {
    ERROR("Main", "Run", "Failed to Awake application");
    return 0;
  }
  //3) initialize device and device context
  if (FAILED(init())) {
    ERROR("Main", "Run", "Failed to initialize device and device context");
    return 0;
  }

  MSG msg = {};
  LARGE_INTEGER freq, prev;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&prev);

  while (WM_QUIT != msg.message) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      LARGE_INTEGER curr;
      QueryPerformanceCounter(&curr);
      float deltaTime = static_cast<float>(curr.QuadPart - prev.QuadPart) /
                        freq.QuadPart;
      prev = curr;
      if (deltaTime > 0.1f)
        deltaTime = 0.1f;

      update(deltaTime);
      render();
    }
  }

  return (int)msg.wParam;
  destroy();
}

HRESULT
BaseApp::init() {
  HRESULT hr = S_OK;

  // DX11 Init
  hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window);
  if (FAILED(hr))
    return hr;
  hr = m_renderTargetView.init(m_device, m_backBuffer,
                               DXGI_FORMAT_R8G8B8A8_UNORM);
  if (FAILED(hr))
    return hr;
  hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height,
                           DXGI_FORMAT_D24_UNORM_S8_UINT,
                           D3D11_BIND_DEPTH_STENCIL, 4, 0);
  if (FAILED(hr))
    return hr;
  hr = m_depthStencilView.init(m_device, m_depthStencil,
                               DXGI_FORMAT_D24_UNORM_S8_UINT);
  if (FAILED(hr))
    return hr;
  hr = m_viewport.init(m_window);
  if (FAILED(hr))
    return hr;

  m_nissanActor = EU::MakeShared<Actor>(m_device);
  if (!m_nissanActor.isNull()) {
    m_model = new Model3D("nissan.fbx", ModelType::FBX);
    std::vector<MeshComponent> modelMeshes = m_model->GetMeshes();

    Texture nissanTexture;
    hr = nissanTexture.init(m_device, "nissan_bake", ExtensionType::PNG);
    std::vector<Texture> textures;
    if (SUCCEEDED(hr))
      textures.push_back(nissanTexture);

    m_nissanActor->setMesh(m_device, modelMeshes);
    m_nissanActor->setTextures(textures);
    m_nissanActor->setName("Nissan GT-R");
    m_nissanActor->getComponent<Transform>()->setTransform(
        EU::Vector3(0, -1, 0), EU::Vector3(0, 0, 0), EU::Vector3(1, 1, 1));

    m_actors.push_back(m_nissanActor);
  }

  // Shaders y Buffers
  std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
  Layout.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                    D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0});
  Layout.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                    D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0});

  hr = m_shaderProgram.init(m_device, "PandoraCoreEngine.fx", Layout);
  if (FAILED(hr))
    return hr;
  hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges));
  if (FAILED(hr))
    return hr;
  hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize));
  if (FAILED(hr))
    return hr;

  // Camara
  m_camera.init(EU::Vector3(0.0f, 2.0f, -6.0f), EU::Vector3(0.0f, 0.0f, 0.0f));
  m_Projection = XMMatrixPerspectiveFovLH(
      XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

  // IMGUI INIT
  m_editor.init(m_window.m_hWnd, m_device.m_device,
                m_deviceContext.m_deviceContext);

  // Outline States
  if (FAILED(initOutlineStates()))
    return E_FAIL;

  return S_OK;
}

HRESULT
BaseApp::initOutlineStates() {
  D3D11_DEPTH_STENCIL_DESC dssDesc;
  ZeroMemory(&dssDesc, sizeof(dssDesc));

  // Write State
  dssDesc.DepthEnable = true;
  dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
  dssDesc.StencilEnable = true;
  dssDesc.StencilReadMask = 0xFF;
  dssDesc.StencilWriteMask = 0xFF;
  dssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  dssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
  dssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  dssDesc.BackFace = dssDesc.FrontFace;

  HRESULT hr =
      m_device.m_device->CreateDepthStencilState(&dssDesc, &m_stencilStateWrite);
  if (FAILED(hr))
    return hr;

  // Mask State
  dssDesc.DepthEnable = false;
  dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  dssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
  dssDesc.BackFace = dssDesc.FrontFace;

  hr = m_device.m_device->CreateDepthStencilState(&dssDesc, &m_stencilStateMask);
  return hr;
}

void
BaseApp::update(float deltaTime) {
  m_camera.update(deltaTime);

  cbNeverChanges.mView = XMMatrixTranspose(m_camera.getViewMatrix());
  m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges,
                          0, 0);

  m_Projection = XMMatrixPerspectiveFovLH(
      XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
  m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr,
                            &cbChangesOnResize, 0, 0);

  for (auto& actor : m_actors) {
    actor->update(deltaTime, m_deviceContext);
  }
}

void
BaseApp::render() {
  float ClearColor[4] = {0.15f, 0.15f, 0.18f, 1.0f};
  m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);
  m_deviceContext.m_deviceContext->ClearDepthStencilView(
      m_depthStencilView.m_depthStencilView,
      D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  m_viewport.render(m_deviceContext);
  m_shaderProgram.render(m_deviceContext);
  m_cbNeverChanges.render(m_deviceContext, 0, 1);
  m_cbChangeOnResize.render(m_deviceContext, 1, 1);

  for (int i = 0; i < m_actors.size(); ++i) {
    if (m_actors[i].isNull())
      continue;
    if (i == m_selectedActorIndex)
      m_deviceContext.m_deviceContext->OMSetDepthStencilState(
          m_stencilStateWrite, 1);
    else
      m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);
    m_actors[i]->render(m_deviceContext);
  }
  m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);

  if (m_selectedActorIndex >= 0 && m_selectedActorIndex < m_actors.size()) {
    renderOutline(m_selectedActorIndex);
  }

  m_editor.render(m_actors, &m_selectedActorIndex);
  m_swapChain.present();
}

void
BaseApp::renderOutline(int actorIndex) {
  auto actor = m_actors[actorIndex];
  if (actor.isNull())
    return;
  auto transform = actor->getComponent<Transform>();
  EU::Vector3 originalScale = transform->getScale();

  transform->setScale(EU::Vector3(originalScale.x * 1.02f,
                                  originalScale.y * 1.02f,
                                  originalScale.z * 1.02f));
  actor->update(0.0f, m_deviceContext);

  m_deviceContext.m_deviceContext->OMSetDepthStencilState(m_stencilStateMask,
                                                          1);
  actor->render(m_deviceContext);

  m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);
  transform->setScale(originalScale);
  actor->update(0.0f, m_deviceContext);
}

void
BaseApp::destroy() {
  if (!m_device.m_device)
    return; 

  m_editor.destroy(); 

  SAFE_RELEASE(m_stencilStateWrite);
  SAFE_RELEASE(m_stencilStateMask);

  if (m_deviceContext.m_deviceContext)
    m_deviceContext.m_deviceContext->ClearState();

  m_cbNeverChanges.destroy();
  m_cbChangeOnResize.destroy();

  m_actors.clear(); 
  if (m_model) {
    delete m_model;
    m_model = nullptr;
  }

  m_shaderProgram.destroy();
  m_depthStencil.destroy();
  m_depthStencilView.destroy();
  m_renderTargetView.destroy();
  m_swapChain.destroy();
  m_backBuffer.destroy();
  m_deviceContext.destroy();
  m_device.destroy();
}

LRESULT
BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  BaseApp* pApp =
      reinterpret_cast<BaseApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  if (pApp) {
    if (pApp->m_editor.WndProc(hWnd, message, wParam, lParam))
      return true;
  }
  switch (message) {
  case WM_CREATE: {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA,
                     (LONG_PTR)pCreate->lpCreateParams);
  }
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}