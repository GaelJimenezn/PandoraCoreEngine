#pragma once
#include "Prerequisites.h"

/**
 * @class Device
 * @brief Wrapper for ID3D11Device, handling resource creation.
 *
 * Provides helper methods to create DirectX 11 resources such as textures,
 * shaders, buffers, and views. It encapsulates the raw ID3D11Device interface.
 */
class
Device {
public:
  Device()  = default;
  ~Device() = default;

  /**
   * @brief Initializes the device (placeholder).
   */
  void
  init();

  void
  update();
  
  void
  render();
  
  /**
   * @brief Releases the device reference.
   */
  void
  destroy();

  /**
   * @brief Creates a Render Target View.
   *
   * @param pResource Raw resource pointer (e.g., Texture2D).
   * @param pDesc     View description (optional).
   * @param ppRTView  Output pointer for the RTV.
   * @return S_OK if successful.
   */
  HRESULT
  CreateRenderTargetView(ID3D11Resource *pResource,
                         const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
                         ID3D11RenderTargetView **ppRTView);

  /**
   * @brief Creates a 2D Texture.
   *
   * @param pDesc        Texture description.
   * @param pInitialData Initial subresource data (optional).
   * @param ppTexture2D  Output pointer for the texture.
   * @return S_OK if successful.
   */
  HRESULT
  CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                  const D3D11_SUBRESOURCE_DATA* pInitialData,
                  ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Creates a Depth Stencil View.
   *
   * @param pResource Raw resource pointer.
   * @param pDesc     View description (optional).
   * @param ppDSView  Output pointer for the DSV.
   * @return S_OK if successful.
   */
  HRESULT
  CreateDepthStencilView(ID3D11Resource* pResource,
                         const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                         ID3D11DepthStencilView** ppDSView);

  /**
   * @brief Creates a Vertex Shader from compiled bytecode.
   *
   * @param pBytecode      Pointer to compiled shader bytecode.
   * @param BytecodeLength Size of the bytecode.
   * @param pLinkage       Class linkage (optional).
   * @param ppVS           Output pointer for the Vertex Shader.
   * @return S_OK if successful.
   */
  HRESULT
  CreateVertexShader(const void* pBytecode,
                     unsigned int BytecodeLength,
                     ID3D11ClassLinkage* pLinkage,
                     ID3D11VertexShader** ppVS);

  /**
   * @brief Creates an Input Layout matching a shader signature.
   *
   * @param pInputElementDescs Array of input element descriptors.
   * @param NumElements        Number of elements.
   * @param pBytecode          Shader bytecode with input signature.
   * @param BytecodeLength     Size of the bytecode.
   * @param ppInputLayout      Output pointer for the Input Layout.
   * @return S_OK if successful.
   */
  HRESULT
  CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                    unsigned int NumElements,
                    const void* pBytecode,
                    unsigned int BytecodeLength,
                    ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Creates a Pixel Shader from compiled bytecode.
   *
   * @param pBytecode      Pointer to compiled shader bytecode.
   * @param BytecodeLength Size of the bytecode.
   * @param pLinkage       Class linkage (optional).
   * @param ppPS           Output pointer for the Pixel Shader.
   * @return S_OK if successful.
   */
  HRESULT
  CreatePixelShader(const void* pBytecode,
                    unsigned int BytecodeLength,
                    ID3D11ClassLinkage* pLinkage,
                    ID3D11PixelShader** ppPS);

  /**
   * @brief Creates a generic Buffer.
   *
   * @param pDesc        Buffer description.
   * @param pInitialData Initial data (optional).
   * @param ppBuffer     Output pointer for the Buffer.
   * @return S_OK if successful.
   */
  HRESULT
  CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
               const D3D11_SUBRESOURCE_DATA* pInitialData,
               ID3D11Buffer** ppBuffer);

  /**
   * @brief Creates a Sampler State.
   *
   * @param pSamplerDesc   Sampler description.
   * @param ppSamplerState Output pointer for the Sampler State.
   * @return S_OK if successful.
   */
  HRESULT
  CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc,
                     ID3D11SamplerState** ppSamplerState);

public:
  ID3D11Device* m_device = nullptr; ///< Pointer to the ID3D11Device.
};