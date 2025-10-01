#pragma once
#include "Prerequisites.h"

/**
 * @class Device
 * @brief Encapsulates the Direct3D 11 device for resource creation.
 * @details This class acts as a factory for all D3D11 resources, such as
 * textures, buffers, and shaders. It wraps the ID3D11Device interface.
 */
class
Device {
public:
  Device() = default;
  ~Device() = default;

  /**
   * @brief Placeholder for device initialization logic.
   */
  void
  init();

  /**
   * @brief Placeholder for per-frame device update logic.
   */
  void
  update();

  /**
   * @brief Placeholder for device-specific rendering logic.
   */
  void
  render();

  /**
   * @brief Releases the underlying ID3D11Device.
   */
  void
  destroy();

  /**
   * @brief Creates a render target view.
   * @param pResource The resource to be used as a render target.
   * @param pDesc An optional descriptor for the render target view.
   * @param ppRTView [out] The created render target view object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createRenderTargetView(ID3D11Resource* pResource,
                         const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                         ID3D11RenderTargetView** ppRTView);

  /**
   * @brief Creates a 2D texture resource.
   * @param pDesc The descriptor for the 2D texture.
   * @param pInitialData Optional data to initialize the texture with.
   * @param ppTexture2D [out] The created 2D texture object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                  const D3D11_SUBRESOURCE_DATA* pInitialData,
                  ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Creates a depth stencil view.
   * @param pResource The resource to be used as a depth-stencil buffer.
   * @param pDesc An optional descriptor for the depth stencil view.
   * @param ppDepthStencilView [out] The created depth stencil view.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createDepthStencilView(ID3D11Resource* pResource,
                         const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                         ID3D11DepthStencilView** ppDepthStencilView);

  /**
   * @brief Creates a vertex shader from compiled bytecode.
   * @param pShaderBytecode A pointer to the compiled shader.
   * @param bytecodeLength The size of the compiled shader in bytes.
   * @param pClassLinkage A pointer to a class linkage interface.
   * @param ppVertexShader [out] The created vertex shader object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createVertexShader(const void* pShaderBytecode,
                     unsigned int bytecodeLength,
                     ID3D11ClassLinkage* pClassLinkage,
                     ID3D11VertexShader** ppVertexShader);

  /**
   * @brief Creates an input layout for the input-assembler stage.
   * @param pInputElementDescs An array of input element descriptions.
   * @param numElements The number of elements in the array.
   * @param pShaderBytecodeWithInputSignature Pointer to the compiled
   * shader with an input signature.
   * @param bytecodeLength The size of the compiled shader in bytes.
   * @param ppInputLayout [out] The created input layout object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                    unsigned int numElements,
                    const void* pShaderBytecodeWithInputSignature,
                    unsigned int bytecodeLength,
                    ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Creates a pixel shader from compiled bytecode.
   * @param pShaderBytecode A pointer to the compiled shader.
   * @param bytecodeLength The size of the compiled shader in bytes.
   * @param pClassLinkage A pointer to a class linkage interface.
   * @param ppPixelShader [out] The created pixel shader object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createPixelShader(const void* pShaderBytecode,
                    unsigned int bytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage,
                    ID3D11PixelShader** ppPixelShader);

  /**
   * @brief Creates a buffer resource (vertex, index, constant, etc.).
   * @param pDesc The descriptor for the buffer.
   * @param pInitialData Optional data to initialize the buffer with.
   * @param ppBuffer [out] The created buffer object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createBuffer(const D3D11_BUFFER_DESC* pDesc,
               const D3D11_SUBRESOURCE_DATA* pInitialData,
               ID3D11Buffer** ppBuffer);

  /**
   * @brief Creates a sampler state for texture sampling.
   * @param pSamplerDesc The descriptor for the sampler state.
   * @param ppSamplerState [out] The created sampler state object.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                     ID3D11SamplerState** ppSamplerState);

public:
  /**
   * @brief Pointer to the core Direct3D 11 device interface.
   * @details This object is responsible for creating all other D3D
   * resources. It is created during initialization and released on destroy.
   */
  ID3D11Device* m_device = nullptr;
};