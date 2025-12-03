#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @class SwapChain
 * @brief Manages the presentation buffers (IDXGISwapChain) in Direct3D 11.
 *
 * Handles the creation and management of front and back buffers. It supports
 * buffer swapping (presentation) and Multisample Anti-Aliasing (MSAA)
 * configuration.
 */
class
SwapChain {
public:
  /**
   * @brief Default constructor.
   */
  SwapChain() = default;

  /**
   * @brief Default destructor.
   */
  ~SwapChain() = default;

  /**
   * @brief Initializes the Swap Chain and retrieves the back buffer.
   *
   * Creates the IDXGISwapChain for the specified window and extracts the
   * back buffer texture for rendering.
   *
   * @param device        Device used for creation.
   * @param deviceContext Device context.
   * @param backBuffer    Texture object to store the back buffer reference.
   * @param window        Target window for presentation.
   * @return S_OK if successful.
   */
  HRESULT
  init(Device& device,
       DeviceContext& deviceContext,
       Texture& backBuffer,
       Window window);

  /**
   * @brief Placeholder for swap chain updates (e.g., resize).
   */
  void
  update();

  /**
   * @brief Placeholder for internal render operations.
   */
  void
  render();

  /**
   * @brief Releases the Swap Chain and DXGI interfaces.
   */
  void
  destroy();

  /**
   * @brief Presents the rendered image to the screen.
   *
   * Calls IDXGISwapChain::Present to swap the front and back buffers.
   */
  void
  present();

public:
  IDXGISwapChain* m_swapChain = nullptr; ///< The DXGI Swap Chain interface.
  D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL; ///< Driver type.

private:
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0; ///< Feature set.
  unsigned int m_sampleCount;      ///< MSAA sample count.
  unsigned int m_qualityLevels;    ///< MSAA quality levels.
  IDXGIDevice* m_dxgiDevice = nullptr;   ///< DXGI Device interface.
  IDXGIAdapter* m_dxgiAdapter = nullptr; ///< DXGI Adapter interface.
  IDXGIFactory* m_dxgiFactory = nullptr; ///< DXGI Factory interface.
};