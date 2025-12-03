#include "BaseApp.h"
#include "ResourceManager.h"

int BaseApp::run(HINSTANCE hInst, int nCmdShow) {
	// Inicializar ventana
	// Pasamos 'WndProc' estático como callback
	if (FAILED(m_window.init(hInst, nCmdShow, WndProc))) {
		return 0;
	}

	// Truco: Guardar puntero a 'this' (la instancia de la App) en los datos de usuario de la ventana.
	// Esto permite recuperar la instancia dentro del WndProc estático para llamar a m_editor.WndProc.
	SetWindowLongPtr(m_window.m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	if (FAILED(init())) {
		return 0;
	}

	// Bucle de mensajes principal
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
			float deltaTime = static_cast<float>(curr.QuadPart - prev.QuadPart) / freq.QuadPart;
			prev = curr;
			update(deltaTime);
			render();
		}
	}
	return (int)msg.wParam;
}

HRESULT BaseApp::init() {
	HRESULT hr = S_OK;

	// 1. Inicialización de Recursos DX11 Básicos
	hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window); 
	if (FAILED(hr)) return hr;

	hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM); 
	if (FAILED(hr)) return hr;

	hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height, 
		DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0); 
	if (FAILED(hr)) return hr;

	hr = m_depthStencilView.init(m_device, m_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT); 
	if (FAILED(hr)) return hr;

	hr = m_viewport.init(m_window); 
	if (FAILED(hr)) return hr;

	// 2. Carga del Actor (Nissan)
	m_nissanActor = EU::MakeShared<Actor>(m_device);

	if (!m_nissanActor.isNull()) {
		// Cargar Malla
		m_model = new Model3D("nissan.fbx", ModelType::FBX);
		std::vector<MeshComponent> modelMeshes = m_model->GetMeshes();
		
		if (modelMeshes.empty()) {
			ERROR("BaseApp", "init", "No meshes found in nissan.fbx");
			return E_FAIL;
		}

		// Cargar Textura
		Texture nissanTexture;
		// Asegúrate de que "nissan_bake.png" esté en la carpeta de Assets o junto al exe
		hr = nissanTexture.init(m_device, "nissan_bake", ExtensionType::PNG); 
		
		std::vector<Texture> textures;
		if (SUCCEEDED(hr)) {
			textures.push_back(nissanTexture);
		}
		else {
			// Si falla la textura, continuamos pero el objeto se verá negro o gris
			OutputDebugStringA("WARNING: Failed to load nissan_bake texture\n");
		}

		// Configurar Actor
		m_nissanActor->setMesh(m_device, modelMeshes);
		m_nissanActor->setTextures(textures);
		m_nissanActor->setName("Nissan GT-R"); // Nombre visible en la Jerarquía
		
		// Configurar Transformación Inicial
		// Usamos escala 1.0f (Original) para asegurar que se vea
		m_nissanActor->getComponent<Transform>()->setTransform(
			EU::Vector3(0.0f, -1.0f, 0.0f),  // Posición: Bajamos un poco (-1 en Y)
			EU::Vector3(0.0f, 0.0f, 0.0f),   // Rotación: 0
			EU::Vector3(1.0f, 1.0f, 1.0f)    // Escala: 1.0 (Tamaño real)
		);

		// Añadir a la lista de actores para que el Editor lo vea
		m_actors.push_back(m_nissanActor);
	}
	else {
		return E_FAIL;
	}

	// 3. Crear Shaders y Layouts
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
	Layout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	hr = m_shaderProgram.init(m_device, "PandoraCoreEngine.fx", Layout); 
	if (FAILED(hr)) return hr;

	// 4. Crear Buffers Constantes (Cámara)
	hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges)); 
	if (FAILED(hr)) return hr;

	hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize)); 
	if (FAILED(hr)) return hr;

	// 5. Inicializar CAMARA DEL EDITOR
	// Posición inicial (0, 2, -6) mirando al origen (0,0,0)
	m_camera.init(EU::Vector3(0.0f, 2.0f, -6.0f), EU::Vector3(0.0f, 0.0f, 0.0f));

	// Configurar matriz de proyección
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

	// 6. Inicializar INTERFAZ DE EDITOR (ImGui Modular)
	m_editor.init(m_window.m_hWnd, m_device.m_device, m_deviceContext.m_deviceContext);
	
	// 7. Inicializar Estados para el EFECTO OUTLINE
	if (FAILED(initOutlineStates())) {
		OutputDebugStringA("CRITICAL ERROR: Failed to initialize Outline States. App might behave unexpectedly.\n");
		return E_FAIL;
	}

	return S_OK;
}

// --- FUNCIÓN CORREGIDA PARA EVITAR EL CRASH DE STENCIL ---
HRESULT BaseApp::initOutlineStates() {
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(dssDesc));

	// 1. ESTADO DE ESCRITURA (Para el objeto seleccionado)
	// Este estado se usa para dibujar el objeto normal y marcar su silueta en el Stencil Buffer con '1'
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dssDesc.StencilEnable = true;
	dssDesc.StencilReadMask = 0xFF;
	dssDesc.StencilWriteMask = 0xFF;

	// Configuración FrontFace (Polígonos que miran a la cámara)
	dssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // <-- IMPORTANTE: Escribir referencia (1)
	dssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;     // Siempre pasar la prueba

	// Configuración BackFace (Polígonos traseros)
	// !! CRUCIAL !!: DirectX requiere que esto tenga valores válidos aunque no se usen (Culling).
	// Si se dejan en 0 (como hace ZeroMemory), el CreateDepthStencilState fallará.
	dssDesc.BackFace = dssDesc.FrontFace; 

	HRESULT hr = m_device.m_device->CreateDepthStencilState(&dssDesc, &m_stencilStateWrite);
	if (FAILED(hr)) return hr;

	// 2. ESTADO DE MASCARA (Para el contorno/outline)
	// Este estado se usa para dibujar el objeto "inflado".
	// Solo dibuja píxeles donde el Stencil NO sea igual a 1 (es decir, fuera de la silueta original).
	dssDesc.DepthEnable = false; // No escribir profundidad (para que sea un efecto visual)
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	
	// FrontFace
	dssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; // <-- Dibuja solo si != referencia

	// BackFace
	dssDesc.BackFace = dssDesc.FrontFace;

	hr = m_device.m_device->CreateDepthStencilState(&dssDesc, &m_stencilStateMask);
	
	return hr;
}

void BaseApp::update(float deltaTime) {
	// 1. Actualizar Cámara (Input del Mouse)
	m_camera.update(deltaTime);

	// 2. Actualizar Buffers Constantes Globales (View Matrix)
	// Obtenemos la matriz de vista actualizada desde la cámara
	cbNeverChanges.mView = XMMatrixTranspose(m_camera.getViewMatrix());
	m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
	
	// Actualizar Proyección (por si la ventana cambió de tamaño)
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
	m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

	// 3. Actualizar Lógica de Actores
	for (auto& actor : m_actors) {
		actor->update(deltaTime, m_deviceContext);
	}
}

void BaseApp::render() {
	// Limpiar pantalla (Gris oscuro para resaltar el editor estilo Dark)
	float ClearColor[4] = { 0.10f, 0.10f, 0.12f, 1.0f }; 
	m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

	// Limpiar Depth y Stencil (Esencial para que el efecto outline funcione cada frame)
	m_deviceContext.m_deviceContext->ClearDepthStencilView(
		m_depthStencilView.m_depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // Stencil a 0

	// Configurar Pipeline común
	m_viewport.render(m_deviceContext);
	m_shaderProgram.render(m_deviceContext);
	m_cbNeverChanges.render(m_deviceContext, 0, 1);
	m_cbChangeOnResize.render(m_deviceContext, 1, 1);

	// --- PASO 1: RENDERIZADO DE OBJETOS (GEOMETRÍA) ---
	for (int i = 0; i < m_actors.size(); ++i) {
		if (m_actors[i].isNull()) continue;

		// Gestión del Stencil para el objeto seleccionado
		if (i == m_selectedActorIndex) {
			// Si es el seleccionado, activamos el estado que escribe '1' en el Stencil
			m_deviceContext.m_deviceContext->OMSetDepthStencilState(m_stencilStateWrite, 1);
		}
		else {
			// Si no, renderizado normal (sin escribir stencil especial)
			m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);
		}

		m_actors[i]->render(m_deviceContext);
	}

	// Restaurar estado de DepthStencil por defecto
	m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);

	// --- PASO 2: EFECTO OUTLINE (POST-GEOMETRÍA) ---
	// Si hay un objeto seleccionado, dibujamos su contorno
	if (m_selectedActorIndex != -1 && m_selectedActorIndex < m_actors.size()) {
		renderOutline(m_selectedActorIndex);
	}

	// --- PASO 3: INTERFAZ DE USUARIO (IMGUI) ---
	m_editor.render(m_actors, &m_selectedActorIndex);

	// Presentar Frame
	m_swapChain.present();
}

void BaseApp::renderOutline(int actorIndex) {
	auto actor = m_actors[actorIndex];
	if (actor.isNull()) return;

	auto transform = actor->getComponent<Transform>();
	
	// Guardar escala original para restaurarla después
	EU::Vector3 originalScale = transform->getScale();

	// 1. Inflar el objeto ligeramente
	// Multiplicamos la escala por 1.03 (3% más grande)
	transform->setScale(EU::Vector3(
		originalScale.x * 1.03f, 
		originalScale.y * 1.03f, 
		originalScale.z * 1.03f
	));
	
	// Forzamos actualización del buffer de constantes del actor (Matriz Mundo) con la nueva escala
	actor->update(0.0f, m_deviceContext); 

	// 2. Configurar Stencil para enmascarar
	// Solo dibuja los píxeles donde el Stencil NO es 1.
	// Como el objeto original (Paso 1) escribió 1 en el centro, el centro queda hueco.
	// Solo se dibuja el borde "inflado".
	m_deviceContext.m_deviceContext->OMSetDepthStencilState(m_stencilStateMask, 1);

	// 3. Dibujar
	// Nota: Al usar el mismo shader, el borde tendrá la textura del objeto estirada.
	// Se verá como un halo del mismo material.
	actor->render(m_deviceContext);

	// 4. Restaurar estado original
	m_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0); // Quitar stencil
	transform->setScale(originalScale); // Restaurar escala real
	actor->update(0.0f, m_deviceContext); // Restaurar matriz mundo
}

void BaseApp::destroy() {
	// Limpiar Editor ImGui
	m_editor.destroy();

	// Liberar Estados de Stencil
	SAFE_RELEASE(m_stencilStateWrite);
	SAFE_RELEASE(m_stencilStateMask);

	if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();

	// Limpiar Buffers y Recursos
	m_cbNeverChanges.destroy();
	m_cbChangeOnResize.destroy();
	
	// Limpiar Actores
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

LRESULT BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Recuperar el puntero a la instancia de BaseApp
	BaseApp* pApp = reinterpret_cast<BaseApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Si tenemos la instancia, delegamos los mensajes de UI al Editor
	if (pApp) {
		if (pApp->m_editor.WndProc(hWnd, message, wParam, lParam))
			return true; // ImGui manejó el evento (ej: click en un botón)
	}

	switch (message) {
	case WM_CREATE:
	{
		// Al crear la ventana, guardamos el puntero 'this' que viene en lpCreateParams
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}