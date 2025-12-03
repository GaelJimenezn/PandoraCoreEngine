#pragma once
#include "Prerequisites.h"

class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Encapsulates a D3D11_VIEWPORT structure.
 *
 * Defines the rectangular area of the render target where drawing occurs.
 * Maps normalized device coordinates (NDC) to window coordinates.
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
   * @brief Initializes the viewport based on window client dimensions.
   *
   * @param window The window determining the rendering area.
   * @return S_OK if successful.
   */
  HRESULT
  init(const Window& window);

  /**
   * @brief Initializes the viewport with explicit dimensions.
   *
   * Sets min/max depth to 0.0 and 1.0 respectively.
   *
   * @param width  Viewport width in pixels.
   * @param height Viewport height in pixels.
   * @return S_OK if successful.
   */
  HRESULT
  init(unsigned int width, unsigned int height);

  /**
   * @brief Placeholder for viewport updates (e.g., on resize).
   */
  void
  update();

  /**
   * @brief Sets the viewport in the Rasterizer stage.
   *
   * @param deviceContext Context used for setting the viewport.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Clean up.
   *
   * No COM resources are managed directly by this class.
   */
  void
  destroy() {}

public:
  /**
   * @brief The native D3D11 viewport structure.
   */
  D3D11_VIEWPORT m_viewport;
};