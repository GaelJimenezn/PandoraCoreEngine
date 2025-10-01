#pragma once
#include "Prerequisites.h"

class 
Device;
class 
DeviceContext;
class 
Window;
class 
Texture;

/**
 * @class SwapChain
 * @brief Manages the front and back buffers for presentation.
 * @details Encapsulates an IDXGISwapChain, handling the chain of buffers
 * used to display rendered images on the screen. It also manages MSAA
 * (Multisample Anti-Aliasing) settings.
 */
class
SwapChain {
public:
  SwapChain() = default;
  ~SwapChain() = default;

  /**
   * @brief Initializes the swap chain for a given window.
   * @param device The graphics device.
   * @param deviceContext The device context for commands.
   * @param backBuffer A texture to receive the back buffer data.
   * @param window The window to which the swap chain will present.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device,
       DeviceContext& deviceContext,
       Texture& backBuffer,
       Window window);

  /**
   * @brief Placeholder for future update logic.
   */
  void
  update();

  /**
   * @brief Placeholder for future rendering logic.
   */
  void
  render();

  /**
   * @brief Releases the swap chain and associated DXGI interfaces.
   */
  void
  destroy();

  /**
   * @brief Presents the back buffer to the screen.
   * @details This swaps the front and back buffers.
   */
  void
  present();

public:
  /**
   * @brief The main IDXGISwapChain interface.
   * @details Responsible for managing and presenting the frame buffers.
   */
  IDXGISwapChain* m_swapChain = nullptr;

  /**
   * @brief The driver type used for device creation.
   * @details (e.g., hardware, reference, or WARP).
   */
  D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;

private:
  /**
   * @brief The Direct3D feature level supported by the hardware.
   */
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

  /**
   * @brief The number of multisamples per pixel for MSAA.
   */
  unsigned int m_sampleCount;

  /**
   * @brief The quality level for the specified sample count.
   */
  unsigned int m_qualityLevels;

  /**
   * @brief The DXGI device interface.
   */
  IDXGIDevice* m_dxgiDevice = nullptr;

  /**
   * @brief The graphics adapter (GPU) interface.
   */
  IDXGIAdapter* m_dxgiAdapter = nullptr;

  /**
   * @brief The factory used to create DXGI objects.
   */
  IDXGIFactory* m_dxgiFactory = nullptr;
};