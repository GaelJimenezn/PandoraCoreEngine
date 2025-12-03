#pragma once
#include "Prerequisites.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ECS/Actor.h"
#include <vector>#pragma once
#include "Prerequisites.h"
#include <cmath>

class EditorCamera {
public:
	EditorCamera() = default;
	~EditorCamera() = default;

	// Inicializa la camara en una posicion mirando al centro
	void init(EU::Vector3 startPos, EU::Vector3 targetPos);

	// Actualiza input y matrices
	void update(float deltaTime);

	// Getters
	XMMATRIX getViewMatrix() const { return m_viewMatrix; }
	XMVECTOR getPosition() const { return m_currentPos; }

private:
	void handleInput(float deltaTime);
	void updateViewMatrix();

private:
	XMMATRIX m_viewMatrix;
	XMVECTOR m_currentPos;
	
	// Orbita
	EU::Vector3 m_focalPoint = { 0.0f, 0.0f, 0.0f };
	float m_distance = 10.0f;
	
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;

	// Configuración
	float m_speedOrbit = 0.005f;
	float m_speedZoom = 2.0f;
	float m_speedPan = 0.02f;
};

namespace Editor {

	class EditorUI {
	public:
		EditorUI() = default;
		~EditorUI() = default;

		// Inicializa ImGui (Contexto, Estilos, Backends)
		void init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);

		// Renderiza la interfaz (Jerarquía, Inspector, etc.)
		// Actualiza el índice del actor seleccionado
		void render(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex);

		// Libera recursos de ImGui
		void destroy();

		// Procesa eventos de input para la UI
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		// Dibuja el panel de lista de objetos
		void drawHierarchy(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex);

		// Dibuja un inspector básico para el objeto seleccionado
		void drawInspector(EU::TSharedPointer<Actor> actor);
		
		// Configura colores y estilo visual
		void setupStyle();
	};
}