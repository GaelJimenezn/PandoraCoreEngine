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
#include "Model3D.h" // Updated: Replaced ModelLoader with Model3D

/**
 * @class BaseApp
 * @brief Serves as the main application class, orchestrating the engine
 * components.
 * @details Initializes all necessary DirectX components (Window, Device,
 * SwapChain, etc.), manages the main application loop (message
 * processing, update, render), and handles resource cleanup.
 */
class
BaseApp {
public:
  /**
   * @brief Constructor.
   * @param hInst Handle to the application instance.
   * @param nCmdShow Window show state flag.
   */
  BaseApp(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Destructor. Calls the destroy() method to clean up resources.
   */
  ~BaseApp();

  /**
   * @brief Enters the main application loop.
   * @details Handles window messages, calls update() and render() each frame.
   * @param hInst Handle to the application instance.
   * @param nCmdShow Window show state flag.
   * @return The application exit code.
   */
  int
  run(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Initializes all DirectX resources and application-specific assets.
   * @return HRESULT indicating success or failure of initialization.
   */
  HRESULT
  init();

  /**
   * @brief Updates application logic and scene state per frame.
   * @param deltaTime Time elapsed since the last frame in seconds.
   */
  void
  update(float deltaTime);

  /**
   * @brief Renders the current scene state to the screen.
   * @details Sets up the pipeline state, binds resources, issues draw calls,
   * and presents the final image.
   */
  void
  render();

  /**
   * @brief Releases all allocated DirectX resources and cleans up.
   */
  void
  destroy();

private:
  /**
   * @brief The window procedure for handling window messages.
   * @details Static because it's used as a callback by the Win32 API.
   * @param hWnd Handle to the window.
   * @param message The message identifier.
   * @param wParam Additional message information.
   * @param lParam Additional message information.
   * @return Result of the message processing.
   */
  static LRESULT CALLBACK
  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
  // Core Engine Components
  Window           m_window;          /**< Manages the application window. */
  Device           m_device;          /**< Creates GPU resources. */
  DeviceContext    m_deviceContext;   /**< Submits commands to the GPU. */
  SwapChain        m_swapChain;       /**< Handles front/back buffer
                                           presentation. */
  Texture          m_backBuffer;      /**< Texture for swap chain's back
                                           buffer. */
  RenderTargetView m_renderTargetView;/**< View for rendering to the back
                                           buffer. */
  Texture          m_depthStencil;    /**< Texture for the depth/stencil
                                           buffer. */
  DepthStencilView m_depthStencilView;/**< View for the depth/stencil
                                           buffer. */
  Viewport         m_viewport;        /**< Defines rendering area on the
                                           target. */
  ShaderProgram    m_shaderProgram;   /**< Manages vertex and pixel shaders. */
  SamplerState     m_samplerState;    /**< Defines texture sampling
                                           behavior. */
  
  // Scene/Object Data
  Model3D* m_model;           /**< The loaded 3D model resource. */
  
  // Changed to vectors to support multiple meshes (FBX structure)
  std::vector<Buffer> m_vertexBuffers; /**< GPU buffers for vertex data. */
  std::vector<Buffer> m_indexBuffers;  /**< GPU buffers for index data. */

  Buffer           m_cbNeverChanges;  /**< Constant buffer for view matrix. */
  Buffer           m_cbChangeOnResize;/**< Constant buffer for projection
                                           matrix. */
  Buffer           m_cbChangesEveryFrame; /**< Constant buffer for world
                                               matrix and color. */
  Texture          m_modelTexture;      

  // Matrices and Rendering State
  XMMATRIX         m_World;           /**< World transformation matrix for
                                           the object. */
  XMMATRIX         m_View;            /**< Camera view matrix. */
  XMMATRIX         m_Projection;      /**< Camera projection matrix. */
  XMFLOAT4         m_vMeshColor;      /**< Color tint applied to the mesh. */

  // Constant Buffer Data Structures (CPU-side copies)
  CBChangeOnResize cbChangesOnResize; /**< CPU copy for projection matrix
                                           updates. */
  CBNeverChanges   cbNeverChanges;    /**< CPU copy for view matrix updates. */
  CBChangesEveryFrame cb;             /**< CPU copy for world matrix/color
                                           updates. */
};