#include "SwapChain.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Texture.h"
#include "Window.h"

HRESULT 
SwapChain::init(Device& device, 
                DeviceContext& deviceContext, 
                Texture& backBuffer, 
                Window window) {
  if (!window.m_hWnd) {
    ERROR("SwapChain", "init", "Invalid window handle.");
    return E_POINTER;
  }

  HRESULT hr = S_OK;

  unsigned int createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] = {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  unsigned int numFeatureLevels = ARRAYSIZE(featureLevels);

  for (unsigned int i = 0; i < numDriverTypes; i++) {
    m_driverType = driverTypes[i];
    hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, 
                           createDeviceFlags, featureLevels, numFeatureLevels, 
                           D3D11_SDK_VERSION, &device.m_device, 
                           &m_featureLevel, &deviceContext.m_deviceContext);
    if (SUCCEEDED(hr)) {
      break;
    }
  }
  if (FAILED(hr)) {
    return hr;
  }

  m_sampleCount = 4;
  hr = device.m_device->CheckMultisampleQualityLevels(
      DXGI_FORMAT_R8G8B8A8_UNORM, m_sampleCount, &m_qualityLevels);
  if (FAILED(hr) || m_qualityLevels == 0) {
    return hr;
  }

  DXGI_SWAP_CHAIN_DESC sd;
  memset(&sd, 0, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = window.m_width;
  sd.BufferDesc.Height = window.m_height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = window.m_hWnd;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.SampleDesc.Count = m_sampleCount;
  sd.SampleDesc.Quality = m_qualityLevels - 1;

  hr = device.m_device->QueryInterface(__uuidof(IDXGIDevice), 
                                        (void**)&m_dxgiDevice);
  if (FAILED(hr)) return hr;

  hr = m_dxgiDevice->GetAdapter(&m_dxgiAdapter);
  if (FAILED(hr)) return hr;

  hr = m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), 
                                (void**)(&m_dxgiFactory));
  if (FAILED(hr)) return hr;

  hr = m_dxgiFactory->CreateSwapChain(device.m_device, &sd, &m_swapChain);
  if (FAILED(hr)) return hr;

  hr = m_swapChain->GetBuffer(0, 
                              __uuidof(ID3D11Texture2D),
                              (void**)(&backBuffer.m_texture));
  if (FAILED(hr)) return hr;

  return S_OK;
}

void 
SwapChain::destroy() {
  SAFE_RELEASE(m_swapChain);
  SAFE_RELEASE(m_dxgiDevice);
  SAFE_RELEASE(m_dxgiAdapter);
  SAFE_RELEASE(m_dxgiFactory);
}

void 
SwapChain::present() {
  if (m_swapChain) {
    m_swapChain->Present(0, 0);
  }
}