#pragma once
#include "Prerequisites.h"

class 
Device;
class 
DeviceContext;
class 
Texture;
class 
DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Manages a Direct3D 11 Render Target View (RTV).
 * @details This class wraps an ID3D11RenderTargetView, which binds a
 * resource (typically a texture) to the output-merger stage to serve as a
 * rendering destination.
 */
class
RenderTargetView {
public:
  RenderTargetView() = default;
  ~RenderTargetView() = default;

  /**
   * @brief Initializes the RTV using the swap chain's back buffer.
   * @param device The device for resource creation.
   * @param backBuffer The texture resource from the swap chain.
   * @param Format The format of the RTV.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device, Texture& backBuffer, DXGI_FORMAT Format);

  /**
   * @brief Initializes the RTV from a generic texture resource.
   * @details This is useful for off-screen rendering techniques like
   * shadow mapping or deferred shading.
   * @param device The device for resource creation.
   * @param inTex The texture to be used as a render target.
   * @param ViewDimension The view dimension (e.g., TEXTURE2D).
   * @param Format The format of the RTV.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device,
       Texture& inTex,
       D3D11_RTV_DIMENSION ViewDimension,
       DXGI_FORMAT Format);

  /**
   * @brief Placeholder for potential future update logic.
   */
  void
  update();

  /**
   * @brief Clears and binds the RTV and an associated DSV.
   * @param deviceContext The device context for rendering commands.
   * @param depthStencilView The depth stencil view to bind.
   * @param numViews The number of RTVs to bind.
   * @param ClearColor The color to clear the RTV with.
   */
  void
  render(DeviceContext& deviceContext,
         DepthStencilView& depthStencilView,
         unsigned int numViews,
         const float ClearColor[4]);
  /**
   * @brief Binds the RTV without a DSV.
   * @param deviceContext The device context for rendering commands.
   * @param numViews The number of RTVs to bind.
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int numViews);

  /**
   * @brief Releases the underlying ID3D11RenderTargetView resource.
   */
  void
  destroy();
private:
  /**
   * @brief The Direct3D 11 render target view resource.
   * @details This pointer becomes valid after a successful call to init()
   * and is set to nullptr when destroy() is called.
   */
  ID3D11RenderTargetView* m_renderTargetView = nullptr;
};