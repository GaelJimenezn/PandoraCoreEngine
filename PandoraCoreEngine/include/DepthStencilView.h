#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;

/**
 * @class DepthStencilView
 * @brief Wraps an ID3D11DepthStencilView for depth/stencil testing.
 *
 * Manages the creation and binding of a Depth Stencil View (DSV), which allows
 * a texture to be used as a depth buffer in the Output Merger stage.
 */
class
DepthStencilView {
public:
  /**
   * @brief Default constructor.
   */
  DepthStencilView() = default;

  /**
   * @brief Default destructor.
   */
  ~DepthStencilView() = default;

  /**
   * @brief Initializes the DSV from a texture.
   *
   * @param device       The device used to create the view.
   * @param depthStencil The texture resource acting as the depth buffer.
   * @param format       DXGI format for the view (e.g., D24_UNORM_S8_UINT).
   * @return S_OK if creation is successful.
   */
  HRESULT
  init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

  /**
   * @brief Placeholder for potential state updates.
   */
  void
  update() {};

  /**
   * @brief Binds this view to the Output Merger stage.
   *
   * @param deviceContext The context used for binding.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Releases the underlying ID3D11DepthStencilView resource.
   */
  void
  destroy();

public:
  /**
   * @brief Pointer to the D3D11 Depth Stencil View interface.
   */
  ID3D11DepthStencilView* m_depthStencilView = nullptr;
};