#pragma once
#include "Prerequisites.h"

class 
Device;
class 
DeviceContext;
class 
Texture;

/**
 * @class DepthStencilView
 * @brief Manages a Direct3D 11 Depth Stencil View (DSV).
 * @details This class encapsulates an ID3D11DepthStencilView, which allows
 * a texture resource to be used as a depth and stencil buffer in the
 * output-merger stage of the graphics pipeline.
 */
class
DepthStencilView {
public:
  DepthStencilView() = default;
  ~DepthStencilView() = default;

  /**
   * @brief Initializes the DSV from a compatible texture resource.
   * @param device The device used for resource creation.
   * @param depthStencil The texture resource to be used as a depth buffer.
   * This texture must be created with D3D11_BIND_DEPTH_STENCIL.
   * @param format The specific format for the view, which must be
   * compatible with the texture's format.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

  /**
   * @brief Binds and clears the depth stencil view.
   * @details Clears the depth and stencil buffers for the current frame.
   * @param deviceContext The device context for issuing rendering commands.
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
   * @brief The Direct3D 11 depth/stencil view resource.
   * @details This pointer is valid after a successful call to init() and
   * becomes nullptr after destroy() is called.
   */
  ID3D11DepthStencilView* m_depthStencilView = nullptr;
};