#include "BaseApp.h"
#include "ResourceManager.h"

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
	if (FAILED(hr)) return hr;

	// Crear render target view
	hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
	if (FAILED(hr)) return hr;

	// Crear textura de depth stencil
	hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height,
		DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0);
	if (FAILED(hr)) return hr;

	// Crear el depth stencil view
	hr = m_depthStencilView.init(m_device, m_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT);
	if (FAILED(hr)) return hr;

	// Crear el m_viewport
	hr = m_viewport.init(m_window);
	if (FAILED(hr)) return hr;

	// --- INICIALIZACIÓN DE ACTORES (MIGRADO) ---
	
	// Crear el Actor
	m_nissanActor = EU::MakeShared<Actor>(m_device);

	if (!m_nissanActor.isNull()) {
		// 1. Cargar el Modelo (Nissan)
		m_model = new Model3D("nissan.fbx", ModelType::FBX);
		std::vector<MeshComponent> modelMeshes = m_model->GetMeshes();
		
		if (modelMeshes.empty()) {
			ERROR("Main", "InitDevice", "No meshes found in nissan.fbx");
			return E_FAIL;
		}

		// 2. Cargar Textura
		Texture nissanTexture;
		hr = nissanTexture.init(m_device, "nissan_bake", ExtensionType::PNG);
		if (FAILED(hr)) {
			ERROR("Main", "InitDevice", "Failed to load nissan_bake texture");
			return hr;
		}
		std::vector<Texture> textures;
		textures.push_back(nissanTexture);

		// 3. Configurar el Actor
		m_nissanActor->setMesh(m_device, modelMeshes);
		m_nissanActor->setTextures(textures);
		m_nissanActor->setName("NissanEntity");
		
		// Configurar Transformación inicial (Escala, Rotación, Posición)
		m_nissanActor->getComponent<Transform>()->setTransform(
			EU::Vector3(0.0f, 1.0f, 0.0f),  // Posición
			EU::Vector3(0.0f, 0.0f, 0.0f),  // Rotación
			EU::Vector3(1.0f, 1.0f, 1.0f)   // Escala
		);

		// Agregar a la lista de actores
		m_actors.push_back(m_nissanActor);
	}
	else {
		ERROR("Main", "InitDevice", "Failed to create Nissan Actor.");
		return E_FAIL;
	}
	// -------------------------------------------

	// Define the input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
	D3D11_INPUT_ELEMENT_DESC position;
	position.SemanticName = "POSITION";
	position.SemanticIndex = 0;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = 0;
	position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
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

	// Create the Shader Program
	hr = m_shaderProgram.init(m_device, "PandoraCoreEngine.fx", Layout);
	if (FAILED(hr)) return hr;

	// Create the constant buffers (Solo los de cámara, el del objeto lo maneja el Actor)
	hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges));
	if (FAILED(hr)) return hr;

	hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize));
	if (FAILED(hr)) return hr;

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	cbNeverChanges.mView = XMMatrixTranspose(m_View);
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

	return S_OK;
}

void BaseApp::update(float deltaTime)
{
	// Update our time logic...
	static float t = 0.0f;
	// (Mantener lógica de tiempo existente...)

	// Actualizar cámara
	cbNeverChanges.mView = XMMatrixTranspose(m_View);
	m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
	
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
	m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

	// --- ACTUALIZAR ACTORES ---
	// La rotación o lógica del Nissan ahora debe hacerse accediendo a su componente Transform
	// Ejemplo de rotación simple:
	if (!m_nissanActor.isNull()) {
		// Rotar un poco cada frame
		auto transform = m_nissanActor->getComponent<Transform>();
		// Puedes acumular rotación aquí o modificar 't' si quieres animación
		// transform->setRotation(EU::Vector3(0.0f, t, 0.0f)); 
	}

	for (auto& actor : m_actors) {
		actor->update(deltaTime, m_deviceContext);
	}
	// --------------------------
}

void 
BaseApp::render() {
	float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);
	m_viewport.render(m_deviceContext);
	m_depthStencilView.render(m_deviceContext);
	m_shaderProgram.render(m_deviceContext);

	// Asignar buffers constantes globales (Cámara)
	m_cbNeverChanges.render(m_deviceContext, 0, 1);
	m_cbChangeOnResize.render(m_deviceContext, 1, 1);
	
	// --- RENDERIZAR ACTORES ---
	// El bucle se encarga de todo (buffers, texturas, shaders locales)
	for (auto& actor : m_actors) {
		actor->render(m_deviceContext);
	}
	// --------------------------

	m_swapChain.present();
}

void 
BaseApp::destroy() {
	if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();

	// Limpieza de buffers globales
	m_cbNeverChanges.destroy();
	m_cbChangeOnResize.destroy();
	
	// Los Actores y sus componentes se destruirán automáticamente o puedes limpiar el vector
	m_actors.clear();
	
	if (m_model) delete m_model;

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
  // Mantener igual...
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