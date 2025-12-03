#pragma once
#include "Prerequisites.h"

// Forward Declarations
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Wraps an ID3D11RenderTargetView (RTV).
 *
 * Manages the resource view allowing a texture (such as the back buffer) to be
 * used as a render target output for the pipeline.
 */
class
RenderTargetView {
public:
  /**
   * @brief Default constructor.
   */
  RenderTargetView() = default;

  /**
   * @brief Default destructor.
   * @details Does not automatically release COM resources; call destroy().
   */
  ~RenderTargetView() = default;

  /**
   * @brief Initializes the RTV from the back buffer texture.
   *
   * @param device     Device used to create the view.
   * @param backBuffer Texture resource from the Swap Chain.
   * @param Format     DXGI format (e.g., DXGI_FORMAT_R8G8B8A8_UNORM).
   * @return S_OK if successful.
   */
  HRESULT
  init(Device& device, Texture& backBuffer, DXGI_FORMAT Format);

  /**
   * @brief Initializes the RTV from a generic texture.
   *
   * @param device        Device used to create the view.
   * @param inTex         Target texture resource.
   * @param ViewDimension View dimension type (e.g., D3D11_RTV_DIMENSION_TEXTURE2D).
   * @param Format        DXGI format.
   * @return S_OK if successful.
   *
   * @note Useful for off-screen render targets (Shadow Maps, G-Buffers).
   */
  HRESULT
  init(Device& device,
       Texture& inTex,
       D3D11_RTV_DIMENSION ViewDimension,
       DXGI_FORMAT Format);

  /**
   * @brief Placeholder for state updates.
   */
  void
  update();

  /**
   * @brief Binds and clears the Render Target and Depth Stencil.
   *
   * Calls OMSetRenderTargets and clears the RTV to the specified color.
   *
   * @param deviceContext    Context used for binding/clearing.
   * @param depthStencilView Depth Stencil View to attach.
   * @param numViews         Number of RTVs to bind (typically 1).
   * @param ClearColor       RGBA float array for the background color.
   */
  void
  render(DeviceContext& deviceContext,
         DepthStencilView& depthStencilView,
         unsigned int numViews,
         const float ClearColor[4]);

  /**
   * @brief Binds the RTV only, without clearing or Depth Stencil.
   *
   * @param deviceContext Context used for binding.
   * @param numViews      Number of RTVs to bind.
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int numViews);

  /**
   * @brief Releases the ID3D11RenderTargetView resource.
   */
  void
  destroy();

private:
  /**
   * @brief D3D11 Render Target View interface.
   */
  ID3D11RenderTargetView* m_renderTargetView = nullptr;
};