#pragma once
#include "Prerequisites.h"

class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Manages the Direct3D 11 viewport configuration.
 * @details Encapsulates a D3D11_VIEWPORT structure, which defines the
 * dimensions and depth range of the area within a render target where
 * the scene will be drawn.
 */
class
Viewport {

public:
  /**
   * @brief Default constructor.
   */
  Viewport() = default;

  /**
   * @brief Default destructor.
   */
  ~Viewport() = default;

  /**
   * @brief Initializes the viewport based on the dimensions of a Window.
   * @param window The application window providing the dimensions.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(const Window& window);

  /**
   * @brief Initializes the viewport with explicit width and height.
   * @param width The width of the viewport in pixels.
   * @param height The height of the viewport in pixels.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(unsigned int width, unsigned int height);

  /**
   * @brief Placeholder for potential viewport update logic (e.g., resizing).
   */
  void
  update();

  /**
   * @brief Binds the viewport configuration to the rasterizer stage.
   * @param deviceContext The device context for issuing commands.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Placeholder for viewport destruction logic (if needed).
   */
  void
  destroy();

public:
  /**
   * @brief The Direct3D 11 viewport structure.
   * @details Stores the top-left corner (TopLeftX, TopLeftY),
   * dimensions (Width, Height), and depth range (MinDepth, MaxDepth).
   */
  D3D11_VIEWPORT m_viewport;
};