#include "Device.h"

void
Device::destroy() {
  SAFE_RELEASE(m_device);
}

HRESULT
Device::createRenderTargetView(ID3D11Resource* pResource,
                               const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                               ID3D11RenderTargetView** ppRTView) {
  if (!pResource) {
    ERROR("Device", "createRenderTargetView", "pResource is nullptr");
    return E_INVALIDARG;
  }
  if (!ppRTView) {
    ERROR("Device", "createRenderTargetView", "ppRTView is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createRenderTargetView", "RTV created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create RTV. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createRenderTargetView", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                        const D3D11_SUBRESOURCE_DATA* pInitialData,
                        ID3D11Texture2D** ppTexture2D) {
  if (!pDesc) {
    ERROR("Device", "createTexture2D", "pDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppTexture2D) {
    ERROR("Device", "createTexture2D", "ppTexture2D is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createTexture2D", "Texture2D created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create Texture2D. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createTexture2D", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createDepthStencilView(ID3D11Resource* pResource,
                               const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                               ID3D11DepthStencilView** ppDepthStencilView) {
  if (!pResource) {
    ERROR("Device", "createDepthStencilView", "pResource is nullptr");
    return E_INVALIDARG;
  }
  if (!ppDepthStencilView) {
    ERROR("Device", "createDepthStencilView", "ppDepthStencilView is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateDepthStencilView(pResource, 
                                                pDesc, 
                                                ppDepthStencilView);
  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createDepthStencilView", "DSV created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create DSV. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createDepthStencilView", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createVertexShader(const void* pShaderBytecode,
                           unsigned int bytecodeLength,
                           ID3D11ClassLinkage* pClassLinkage,
                           ID3D11VertexShader** ppVertexShader) {
  if (!pShaderBytecode) {
    ERROR("Device", "createVertexShader", "pShaderBytecode is nullptr");
    return E_INVALIDARG;
  }
  if (!ppVertexShader) {
    ERROR("Device", "createVertexShader", "ppVertexShader is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateVertexShader(pShaderBytecode,
                                            bytecodeLength,
                                            pClassLinkage,
                                            ppVertexShader);
  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createVertexShader", "VS created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create Vertex Shader. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createVertexShader", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                          unsigned int numElements,
                          const void* pShaderBytecodeWithInputSignature,
                          unsigned int bytecodeLength,
                          ID3D11InputLayout** ppInputLayout) {
  if (!pInputElementDescs) {
    ERROR("Device", "createInputLayout", "pInputElementDescs is nullptr");
    return E_INVALIDARG;
  }
  if (!ppInputLayout) {
    ERROR("Device", "createInputLayout", "ppInputLayout is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateInputLayout(pInputElementDescs,
                                           numElements,
                                           pShaderBytecodeWithInputSignature,
                                           bytecodeLength,
                                           ppInputLayout);
  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createInputLayout", "Input Layout created!");
  }
  else {
    std::string errorMsg = "Failed to create Input Layout. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createInputLayout", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createPixelShader(const void* pShaderBytecode,
                          unsigned int bytecodeLength,
                          ID3D11ClassLinkage* pClassLinkage,
                          ID3D11PixelShader** ppPixelShader) {
  if (!pShaderBytecode) {
    ERROR("Device", "createPixelShader", "pShaderBytecode is nullptr");
    return E_INVALIDARG;
  }
  if (!ppPixelShader) {
    ERROR("Device", "createPixelShader", "ppPixelShader is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreatePixelShader(pShaderBytecode,
                                           bytecodeLength,
                                           pClassLinkage,
                                           ppPixelShader);
  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createPixelShader", "PS created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create Pixel Shader. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createPixelShader", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                           ID3D11SamplerState** ppSamplerState) {
  if (!pSamplerDesc) {
    ERROR("Device", "createSamplerState", "pSamplerDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppSamplerState) {
    ERROR("Device", "createSamplerState", "ppSamplerState is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateSamplerState(pSamplerDesc, ppSamplerState);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createSamplerState", "Sampler State created!");
  }
  else {
    std::string errorMsg = "Failed to create Sampler State. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createSamplerState", errorMsg.c_str());
  }

  return hr;
}

HRESULT
Device::createBuffer(const D3D11_BUFFER_DESC* pDesc,
                     const D3D11_SUBRESOURCE_DATA* pInitialData,
                     ID3D11Buffer** ppBuffer) {
  if (!pDesc) {
    ERROR("Device", "createBuffer", "pDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppBuffer) {
    ERROR("Device", "createBuffer", "ppBuffer is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "createBuffer", "Buffer created successfully!");
  }
  else {
    std::string errorMsg = "Failed to create Buffer. HRESULT: " 
                         + std::to_string(hr);
    ERROR("Device", "createBuffer", errorMsg.c_str());
  }

  return hr;
}