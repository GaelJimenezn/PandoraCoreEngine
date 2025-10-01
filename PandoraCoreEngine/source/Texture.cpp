#include "Texture.h"
#include "Device.h"
#include "DeviceContext.h"

HRESULT 
Texture::init(Device& device, 
              const std::string& textureName, 
              ExtensionType extensionType) {
  return E_NOTIMPL;
}

HRESULT 
Texture::init(Device& device, 
              unsigned int width, 
              unsigned int height, 
              DXGI_FORMAT format, 
              unsigned int bindFlags, 
              unsigned int sampleCount, 
              unsigned int qualityLevels) {
  if (!device.m_device) {
    ERROR("Texture", "init", "Device is null.");
    return E_POINTER;
  }
  if (width == 0 || height == 0) {
    ERROR("Texture", "init", "Width and height must be > 0");
    return E_INVALIDARG;
  }

  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = format;
  desc.SampleDesc.Count = sampleCount;
  desc.SampleDesc.Quality = qualityLevels;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = bindFlags;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  HRESULT hr = device.createTexture2D(&desc, nullptr, &m_texture);

  if (FAILED(hr)) {
    std::string errorMsg = "Failed to create texture. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Texture", "init", errorMsg.c_str());
    return hr;
  }

  return S_OK;
}

HRESULT 
Texture::init(Device& device, Texture& textureRef, DXGI_FORMAT format) {
  if (!device.m_device) {
    ERROR("Texture", "init", "Device is null.");
    return E_POINTER;
  }
  if (!textureRef.m_texture) {
    ERROR("Texture", "init", "Texture is null.");
    return E_POINTER;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  srvDesc.Texture2D.MostDetailedMip = 0;

  HRESULT hr = device.m_device->CreateShaderResourceView(
      textureRef.m_texture, &srvDesc, &m_textureFromImg);

  if (FAILED(hr)) {
    std::string errorMsg = "Failed to create SRV. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Texture", "init", errorMsg.c_str());
    return hr;
  }

  return S_OK;
}

void 
Texture::update() {
}

void 
Texture::render(DeviceContext& deviceContext, 
                unsigned int startSlot, 
                unsigned int numViews) {
  if (!deviceContext.m_deviceContext) {
    ERROR("Texture", "render", "Device Context is null.");
    return;
  }

  if (m_textureFromImg) {
    deviceContext.psSetShaderResources(startSlot, numViews, &m_textureFromImg);
  }
}

void 
Texture::destroy() {
  SAFE_RELEASE(m_texture);
  SAFE_RELEASE(m_textureFromImg);
}