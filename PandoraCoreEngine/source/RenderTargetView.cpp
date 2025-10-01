#include "RenderTargetView.h"
#include "Device.h"
#include "Texture.h"
#include "DeviceContext.h"
#include "DepthStencilView.h"

HRESULT
RenderTargetView::init(Device& device, 
                       Texture& backBuffer, 
                       DXGI_FORMAT format) {
  if (!device.m_device) {
    ERROR("RenderTargetView", "init",	"Device is nullptr.");
    return E_POINTER;
  }
  if (!backBuffer.m_texture) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

  HRESULT hr = device.createRenderTargetView(backBuffer.m_texture, 
                                             &desc, 
                                             &m_renderTargetView);
  if (FAILED(hr)) {
    std::string errorMsg = "Failed to create RTV. HRESULT: " 
                         + std::to_string(hr);
    ERROR("RenderTargetView", "init", errorMsg.c_str());
    return hr;
  }

  return S_OK;
}

HRESULT 
RenderTargetView::init(Device& device, 
                       Texture& inTex, 
                       D3D11_RTV_DIMENSION viewDimension, 
                       DXGI_FORMAT format) {
  if (!device.m_device) {
    ERROR("RenderTargetView", "init", "Device is nullptr.");
    return E_POINTER;
  }
  if (!inTex.m_texture) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = format;
  desc.ViewDimension = viewDimension;

  HRESULT hr = device.createRenderTargetView(inTex.m_texture, 
                                             &desc, 
                                             &m_renderTargetView);
  if (FAILED(hr)) {
    std::string errorMsg = "Failed to create RTV. HRESULT: " 
                         + std::to_string(hr);
    ERROR("RenderTargetView", "init", errorMsg.c_str());
    return hr;
  }

  return S_OK;
}

void 
RenderTargetView::render(DeviceContext& deviceContext, 
                         DepthStencilView& depthStencilView, 
                         unsigned int numViews, 
                         const float clearColor[4]) {
  if (!deviceContext.m_deviceContext) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_renderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }

  deviceContext.clearRenderTargetView(m_renderTargetView, clearColor);

  deviceContext.omSetRenderTargets(numViews,
                                   &m_renderTargetView,
                                   depthStencilView.m_depthStencilView);
}

void 
RenderTargetView::render(DeviceContext& deviceContext, unsigned int numViews) {
  if (!deviceContext.m_deviceContext) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_renderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }
  deviceContext.omSetRenderTargets(numViews, 
                                   &m_renderTargetView, 
                                   nullptr);
}

void
RenderTargetView::destroy() {
  SAFE_RELEASE(m_renderTargetView);
}