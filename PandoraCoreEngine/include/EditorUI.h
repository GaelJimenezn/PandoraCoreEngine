#pragma once
#include "Prerequisites.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ECS/Actor.h"
#include <vector>

namespace Editor {

	class EditorUI {
	public:
		EditorUI() = default;
		~EditorUI() = default;

		// Inicializa ImGui (Contexto, Estilos, Backends)
		void init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);

		// Renderiza la interfaz (Jerarquía, Inspector, etc.)
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