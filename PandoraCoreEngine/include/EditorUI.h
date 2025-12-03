#pragma once
#include "Prerequisites.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ECS/Actor.h"
#include <vector>

namespace Editor {

/**
 * @class EditorUI
 * @brief Manages the ImGui-based editor interface.
 *
 * Handles the initialization, rendering, and input processing for the
 * engine's editor layer. This includes the Scene Hierarchy panel and the
 * Inspector panel for modifying actor properties.
 */
class
EditorUI {
public:
  /**
   * @brief Default constructor.
   */
  EditorUI() = default;

  /**
   * @brief Default destructor.
   */
  ~EditorUI() = default;

  /**
   * @brief Initializes the ImGui context and backends.
   *
   * Configures style, platform bindings (Win32), and renderer bindings
   * (DirectX 11).
   *
   * @param hWnd    Handle to the application window.
   * @param device  Pointer to the D3D11 Device.
   * @param context Pointer to the D3D11 Device Context.
   */
  void
  init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);

  /**
   * @brief Renders the editor panels.
   *
   * Issues ImGui commands to draw the Hierarchy and Inspector windows based
   * on the current scene state.
   *
   * @param actors        Reference to the list of active actors in the scene.
   * @param selectedIndex Pointer to the currently selected actor index.
   */
  void
  render(std::vector<EU::TSharedPointer<Actor>>& actors, int* selectedIndex);

  /**
   * @brief Shuts down ImGui and releases resources.
   */
  void
  destroy();

  /**
   * @brief Processes Windows messages for UI interaction.
   *
   * Forward system events (mouse, keyboard) to the ImGui handler.
   *
   * @param hWnd   Window handle.
   * @param msg    Message identifier.
   * @param wParam Message parameter.
   * @param lParam Message parameter.
   * @return The result of the message processing.
   */
  LRESULT
  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
  /**
   * @brief Draws the Scene Hierarchy panel.
   *
   * Displays a list of all actors, allowing selection.
   *
   * @param actors        List of actors to display.
   * @param selectedIndex Pointer to update when an actor is clicked.
   */
  void
  drawHierarchy(std::vector<EU::TSharedPointer<Actor>>& actors,
                int* selectedIndex);

  /**
   * @brief Draws the Inspector panel for the selected actor.
   *
   * Exposes components (Transform, Mesh, etc.) for editing.
   *
   * @param actor The currently selected actor.
   */
  void
  drawInspector(EU::TSharedPointer<Actor> actor);
		
  /**
   * @brief Configures custom visual styles and colors for the editor.
   */
  void
  setupStyle();
};
}