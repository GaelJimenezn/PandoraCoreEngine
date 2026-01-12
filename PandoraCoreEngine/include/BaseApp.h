#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"

// --- NEW MODULES ---
#include "EditorUI.h" 
#include "EditorCamera.h"

/**
 * @class BaseApp
 * @brief Core application class managing the main loop and graphics pipeline.
 *
 * This class serves as the entry point for the engine, handling window
 * creation, device initialization, the main message loop, and the high-level
 * render cycle (update/render). It also manages core resources like the
 * swap chain, depth buffers, and editor integration.
 */
class
BaseApp {
public:
  /**
   * @brief Default constructor.
   */
  BaseApp() = default;

  /**
   * @brief Destructor.
   *
   * Ensures all resources are properly released by calling destroy().
   */
  ~BaseApp() { destroy(); }

  HRESULT
  awake();

  /**
   * @brief Executes the main application loop.
   *
   * Initializes the window and enters the message pump.
   *
   * @param hInst    Handle to the current application instance.
   * @param nCmdShow Control flags for how the window is to be shown.
   * @return The exit code from the WM_QUIT message.
   */
  int
  run(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Initializes the engine subsystems.
   *
   * Sets up the window, graphics device, swap chain, views, and initial
   * scene resources.
   *
   * @return S_OK if initialization succeeds, otherwise an error code.
   */
  HRESULT
  init();

  /**
   * @brief Updates the application logic for the current frame.
   *
   * Handles input processing, camera updates, and actor simulations.
   *
   * @param deltaTime Time elapsed since the last frame in seconds.
   */
  void
  update(float deltaTime);

  /**
   * @brief Renders the current frame.
   *
   * Clears buffers, draws the scene geometry, renders the editor UI, and
   * presents the swap chain.
   */
  void
  render();

  /**
   * @brief Releases all allocated resources.
   *
   * Cleans up Direct3D objects, window handles, and memory allocations.
   */
  void
  destroy();

private:
  /**
   * @brief Windows message handler callback.
   *
   * Processes system events such as input, resizing, and window closure.
   *
   * @param hWnd    Handle to the window.
   * @param message The message identifier.
   * @param wParam  Additional message information.
   * @param lParam  Additional message information.
   * @return Result of the message processing.
   */
  static LRESULT CALLBACK
  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  
  /**
   * @brief Initializes depth-stencil states required for outline rendering.
   * @return S_OK if successful.
   */
  HRESULT
  initOutlineStates();

  /**
   * @brief Renders a visual outline around the selected actor.
   *
   * Uses stencil masking to draw a scaled or colored silhouette.
   *
   * @param actorIndex Index of the actor to outline in the m_actors list.
   */
  void
  renderOutline(int actorIndex);

private:
  Window                              m_window;
  Device                              m_device;
  DeviceContext                       m_deviceContext;
  SwapChain                           m_swapChain;
  Texture                             m_backBuffer;
  RenderTargetView                    m_renderTargetView;
  Texture                             m_depthStencil;
  DepthStencilView                    m_depthStencilView;
  Viewport                            m_viewport;
  ShaderProgram                       m_shaderProgram;

  Buffer                              m_cbNeverChanges;
  Buffer                              m_cbChangeOnResize;
  
  // Camera handles view matrix now
  // XMMATRIX                         m_View; 
  XMMATRIX                            m_Projection;

  Model3D* m_model;

  std::vector<EU::TSharedPointer<Actor>> m_actors;
  EU::TSharedPointer<Actor>              m_nissanActor;

  CBChangeOnResize                    cbChangesOnResize;
  CBNeverChanges                      cbNeverChanges;

  // --- EDITOR ---
  Editor::EditorUI                    m_editor;
  EditorCamera                        m_camera;
  int                                 m_selectedActorIndex = -1;

  // DX11 States for Outline effect
  ID3D11DepthStencilState* m_stencilStateWrite = nullptr;
  ID3D11DepthStencilState* m_stencilStateMask = nullptr;
};