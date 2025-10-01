#pragma once
#include "Prerequisites.h"

/**
 * @class DeviceContext
 * @brief Wraps the Direct3D 11 device context for command submission.
 * @details This class encapsulates the ID3D11DeviceContext, which is
 * responsible for generating rendering commands, binding resources to the
 * graphics pipeline, and executing draw calls.
 */
class
DeviceContext {
public:
  DeviceContext() = default;
  ~DeviceContext() = default;

  /**
   * @brief Placeholder for context initialization logic.
   */
  void
  init();

  /**
   * @brief Placeholder for per-frame context update logic.
   */
  void
  update();

  /**
   * @brief Placeholder for context-specific rendering logic.
   */
  void
  render();

  /**
   * @brief Releases the underlying ID3D11DeviceContext.
   */
  void
  destroy();

  /**
   * @brief Binds an array of viewports to the rasterizer stage.
   */
  void
  rsSetViewports(unsigned int numViewports,
                 const D3D11_VIEWPORT* pViewports);

  /**
   * @brief Binds shader resource views to the pixel shader stage.
   */
  void
  psSetShaderResources(unsigned int startSlot,
                       unsigned int numViews,
                       ID3D11ShaderResourceView* const* ppShaderResourceViews);

  /**
   * @brief Binds an input-layout object to the input-assembler stage.
   */
  void
  iaSetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Binds a vertex shader to the vertex shader stage.
   */
  void
  vsSetShader(ID3D11VertexShader* pVertexShader,
              ID3D11ClassInstance* const* ppClassInstances,
              unsigned int numClassInstances);

  /**
   * @brief Binds a pixel shader to the pixel shader stage.
   */
  void
  psSetShader(ID3D11PixelShader* pPixelShader,
              ID3D11ClassInstance* const* ppClassInstances,
              unsigned int numClassInstances);

  /**
   * @brief Updates the data in a subresource (e.g., a constant buffer).
   */
  void
  updateSubresource(ID3D11Resource* pDstResource,
                    unsigned int dstSubresource,
                    const D3D11_BOX* pDstBox,
                    const void* pSrcData,
                    unsigned int srcRowPitch,
                    unsigned int srcDepthPitch);

  /**
   * @brief Binds an array of vertex buffers to the input-assembler stage.
   */
  void
  iaSetVertexBuffers(unsigned int startSlot,
                     unsigned int numBuffers,
                     ID3D11Buffer* const* ppVertexBuffers,
                     const unsigned int* pStrides,
                     const unsigned int* pOffsets);
  /**
   * @brief Binds an index buffer to the input-assembler stage.
   */
  void
  iaSetIndexBuffer(ID3D11Buffer* pIndexBuffer,
                   DXGI_FORMAT format,
                   unsigned int offset);
  /**
   * @brief Binds an array of sampler states to the pixel shader stage.
   */
  void
  psSetSamplers(unsigned int startSlot,
                unsigned int numSamplers,
                ID3D11SamplerState* const* ppSamplers);

  /**
   * @brief Sets the rasterizer state for the rasterizer stage.
   */
  void
  rsSetState(ID3D11RasterizerState* pRasterizerState);

  /**
   * @brief Sets the blend state for the output-merger stage.
   */
  void
  omSetBlendState(ID3D11BlendState* pBlendState,
                  const float blendFactor[4],
                  unsigned int sampleMask);

  /**
   * @brief Binds render targets and a depth-stencil view to the OM stage.
   */
  void
  omSetRenderTargets(unsigned int numViews,
                     ID3D11RenderTargetView* const* ppRenderTargetViews,
                     ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Binds the primitive topology to the input-assembler stage.
   */
  void
  iaSetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

  /**
   * @brief Clears a render-target view to a specified color.
   */
  void
  clearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
                        const float colorRGBA[4]);

  /**
   * @brief Clears a depth-stencil view.
   */
  void
  clearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                        unsigned int clearFlags,
                        float depth,
                        UINT8 stencil);
  /**
   * @brief Binds constant buffers to the vertex shader stage.
   */
  void
  vsSetConstantBuffers(unsigned int startSlot,
                       unsigned int numBuffers,
                       ID3D11Buffer* const* ppConstantBuffers);
  /**
   * @brief Binds constant buffers to the pixel shader stage.
   */
  void
  psSetConstantBuffers(unsigned int startSlot,
                       unsigned int numBuffers,
                       ID3D11Buffer* const* ppConstantBuffers);
  /**
   * @brief Submits an indexed-primitive draw call.
   */
  void
  drawIndexed(unsigned int indexCount,
              unsigned int startIndexLocation,
              int baseVertexLocation);

public:
  /**
   * @brief Pointer to the immediate context of Direct3D 11.
   * @details This interface is used to issue all rendering commands. It is
   * acquired during initialization and released upon destruction.
   */
  ID3D11DeviceContext* m_deviceContext = nullptr;
};