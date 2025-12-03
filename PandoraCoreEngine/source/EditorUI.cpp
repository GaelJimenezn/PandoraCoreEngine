#include "EditorUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Editor {

	void EditorUI::init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		setupStyle();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device, context);
	}

	void EditorUI::setupStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 10.0f;
		style.FrameRounding = 5.0f;
		style.PopupRounding = 5.0f;
		style.WindowBorderSize = 0.0f;
		
		ImVec4* colors = style.Colors;
		// Fondo oscuro translúcido
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.90f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
		
		// Acentos Neon (Violeta)
		ImVec4 accent = ImVec4(0.60f, 0.10f, 0.90f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(accent.x, accent.y, accent.z, 0.5f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(accent.x, accent.y, accent.z, 0.8f);
		colors[ImGuiCol_HeaderActive] = accent;
		colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
		colors[ImGuiCol_ButtonHovered] = accent;
		colors[ImGuiCol_ButtonActive] = accent;
		colors[ImGuiCol_CheckMark] = accent;
		colors[ImGuiCol_SliderGrab] = accent;
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8f, 0.2f, 1.0f, 1.0f);
	}

	void EditorUI::render(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Jerarquía (Izquierda)
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, 500), ImGuiCond_FirstUseEver);
		ImGui::Begin("Jerarquia", nullptr, ImGuiWindowFlags_NoCollapse);
		drawHierarchy(actors, selectedIndex);
		ImGui::End();

		// 2. Inspector (Derecha) - Siempre visible
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 310, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
		
		EU::TSharedPointer<Actor> currentActor;
		if (*selectedIndex >= 0 && *selectedIndex < actors.size()) {
			currentActor = actors[*selectedIndex];
		}
		drawInspector(currentActor);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUI::drawHierarchy(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex) {
		ImGui::TextDisabled("ESCENA");
		ImGui::Separator();

		for (int i = 0; i < actors.size(); ++i) {
			if (actors[i].isNull()) continue;

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			if (*selectedIndex == i) flags |= ImGuiTreeNodeFlags_Selected;

			std::string name = actors[i]->getName();
			if (name.empty()) name = "Actor " + std::to_string(i);

			// Icono falso [Mesh]
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "[Mesh]"); ImGui::SameLine();
			ImGui::TreeNodeEx((void*)(intptr_t)i, flags, name.c_str());

			if (ImGui::IsItemClicked()) *selectedIndex = i;
		}

		// Deseleccionar al hacer click en vacío
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			*selectedIndex = -1;
		}
	}

	void EditorUI::drawInspector(EU::TSharedPointer<Actor> actor) {
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);
		
		if (!actor.isNull()) {
			ImGui::TextColored(ImVec4(0.6f, 0.1f, 0.9f, 1.0f), "PROPIEDADES");
			ImGui::Separator();
			ImGui::Spacing();

			// Nombre
			char buffer[128];
			strcpy_s(buffer, actor->getName().c_str());
			if (ImGui::InputText("Nombre", buffer, sizeof(buffer))) {
				actor->setName(std::string(buffer));
			}

			ImGui::Spacing();
			ImGui::TextDisabled("Transformacion");
			ImGui::Separator();

			auto t = actor->getComponent<Transform>();
			if (t) {
				EU::Vector3 pos = t->getPosition();
				float p[3] = { pos.x, pos.y, pos.z };
				if (ImGui::DragFloat3("Posicion", p, 0.1f)) t->setPosition({ p[0], p[1], p[2] });

				EU::Vector3 rot = t->getRotation();
				float r[3] = { rot.x, rot.y, rot.z };
				if (ImGui::DragFloat3("Rotacion", r, 0.1f)) t->setRotation({ r[0], r[1], r[2] });

				EU::Vector3 scale = t->getScale();
				float s[3] = { scale.x, scale.y, scale.z };
				if (ImGui::DragFloat3("Escala", s, 0.01f)) t->setScale({ s[0], s[1], s[2] });
			}
		}
		else {
			// Mensaje cuando no hay nada seleccionado
			ImGui::Spacing();
			ImGui::TextDisabled("Ningun objeto seleccionado.");
		}
		ImGui::End();
	}

	void EditorUI::destroy() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	LRESULT EditorUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}