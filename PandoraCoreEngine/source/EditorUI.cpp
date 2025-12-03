#include "EditorUI.h"

// Definición externa necesaria para que Windows envíe los inputs a ImGui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace 
Editor {

	void 
	EditorUI::init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		// Habilitar navegación por teclado (Importante para escribir en los inputs)
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		
		setupStyle();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device, context);
	}

	void 
	EditorUI::setupStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 8.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.WindowBorderSize = 0.0f;
		
		// Colores estilo Dark Theme
		ImVec4* colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.95f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
		
		// Acentos (Violeta)
		ImVec4 accent = ImVec4(0.60f, 0.10f, 0.90f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(accent.x, accent.y, accent.z, 0.4f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(accent.x, accent.y, accent.z, 0.7f);
		colors[ImGuiCol_HeaderActive] = accent;
		colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = accent;
		colors[ImGuiCol_ButtonActive] = ImVec4(accent.x * 1.2f, accent.y * 1.2f, accent.z * 1.2f, 1.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.32f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.42f, 1.0f);
		colors[ImGuiCol_CheckMark] = accent;
	}

	void 
	EditorUI::render(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Jerarquía
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Jerarquia", nullptr);
		drawHierarchy(actors, selectedIndex);
		ImGui::End();

		// 2. Inspector
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 310, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
		
		EU::TSharedPointer<Actor> currentActor;
		if (*selectedIndex >= 0 && *selectedIndex < actors.size()) {
			currentActor = actors[*selectedIndex];
		}
		drawInspector(currentActor);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void 
	EditorUI::drawHierarchy(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex) {
		ImGui::TextDisabled("ESCENA");
		ImGui::Separator();
		ImGui::Spacing();

		for (int i = 0; i < actors.size(); ++i) {
			if (actors[i].isNull()) continue;

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (*selectedIndex == i) flags |= ImGuiTreeNodeFlags_Selected;

			std::string name = actors[i]->getName();
			if (name.empty()) name = "Actor " + std::to_string(i);

			// Icono y nombre
			ImGui::AlignTextToFramePadding();
			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, name.c_str());

			if (ImGui::IsItemClicked()) {
				*selectedIndex = i;
			}
		}

		// Deseleccionar clickeando vacio
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
			*selectedIndex = -1;
		}
	}

	void 
	EditorUI::drawInspector(EU::TSharedPointer<Actor> actor) {
		ImGui::Begin("Inspector", nullptr);
		
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
			ImGui::Separator();
			ImGui::TextDisabled("TRANSFORM");
			ImGui::Spacing();

			auto t = actor->getComponent<Transform>();
			if (t) {
				EU::Vector3 pos = t->getPosition();
				float p[3] = { pos.x, pos.y, pos.z };

				EU::Vector3 rot = t->getRotation();
				float r[3] = { rot.x, rot.y, rot.z };

				EU::Vector3 scale = t->getScale();
				float s[3] = { scale.x, scale.y, scale.z };

				// USAMOS InputFloat3 PARA PODER ESCRIBIR
				ImGui::Text("Posicion");
				if (ImGui::InputFloat3("##pos", p, "%.2f")) t->setPosition({ p[0], p[1], p[2] });

				ImGui::Text("Rotacion");
				if (ImGui::InputFloat3("##rot", r, "%.2f")) t->setRotation({ r[0], r[1], r[2] });

				ImGui::Text("Escala");
				if (ImGui::InputFloat3("##scl", s, "%.2f")) t->setScale({ s[0], s[1], s[2] });
			}
		}
		else {
			ImGui::Spacing();
			ImGui::TextDisabled("Selecciona un objeto...");
		}
		ImGui::End();
	}

	void 
	EditorUI::destroy() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	LRESULT 
	EditorUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}