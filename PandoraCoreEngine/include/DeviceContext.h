#pragma once
#include "Prerequisites.h"

/**
 * @class DeviceContext
 * @brief Wrapper for ID3D11DeviceContext, handling rendering commands.
 *
 * Encapsulates the immediate context used for issuing rendering commands such
 * as drawing primitives, setting state (shaders, buffers, views), and updating
 * resources.
 */
class
DeviceContext {
public:
  DeviceContext()  = default;
  ~DeviceContext() = default;

  /**
   * @brief Initializes the device context.
   */
  void
  init();

  /**
   * @brief Placeholder for context updates.
   */
  void
  update();

  /**
   * @brief Placeholder for rendering operations.
   */
  void
  render();

  /**
   * @brief Releases the ID3D11DeviceContext reference.
   */
  void
  destroy();

  /**
   * @brief Sets the viewports for the Rasterizer stage.
   *
   * @param NumViewports Number of viewports.
   * @param pViewports   Pointer to viewport array.
   */
  void
  RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT *pViewports);

  /**
   * @brief Sets Shader Resource Views for the Pixel Shader stage.
   *
   * @param StartSlot Start slot index.
   * @param NumViews  Number of views to bind.
   * @param ppSRVs    Array of Shader Resource View pointers.
   */
  void
  PSSetShaderResources(unsigned int StartSlot,
                       unsigned int NumViews,
                       ID3D11ShaderResourceView* const* ppSRVs);

  /**
   * @brief Sets the active Input Layout.
   *
   * @param pInputLayout Pointer to the Input Layout.
   */
  void
  IASetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Sets the Vertex Shader.
   *
   * @param pVertexShader     Pointer to Vertex Shader.
   * @param ppClassInstances  Array of class instances (optional).
   * @param NumClassInstances Number of class instances.
   */
  void
  VSSetShader(ID3D11VertexShader* pVertexShader,
              ID3D11ClassInstance* const* ppClassInstances,
              unsigned int NumClassInstances);

  /**
   * @brief Sets the Pixel Shader.
   *
   * @param pPixelShader      Pointer to Pixel Shader.
   * @param ppClassInstances  Array of class instances (optional).
   * @param NumClassInstances Number of class instances.
   */
  void
  PSSetShader(ID3D11PixelShader *pPixelShader,
              ID3D11ClassInstance *const *ppClassInstances,
              unsigned int NumClassInstances);

  /**
   * @brief Updates a subresource with new data from the CPU.
   *
   * @param pDstResource   Destination resource.
   * @param DstSubresource Subresource index.
   * @param pDstBox        Destination box (optional).
   * @param pSrcData       Source data pointer.
   * @param SrcRowPitch    Row pitch of source data.
   * @param SrcDepthPitch  Depth pitch of source data.
   */
  void
  UpdateSubresource(ID3D11Resource* pDstResource,
                    unsigned int DstSubresource,
                    const D3D11_BOX* pDstBox,
                    const void* pSrcData,
                    unsigned int SrcRowPitch,
                    unsigned int SrcDepthPitch);

  /**
   * @brief Binds Vertex Buffers to the Input Assembler stage.
   *
   * @param StartSlot       Start slot index.
   * @param NumBuffers      Number of buffers.
   * @param ppVertexBuffers Array of buffer pointers.
   * @param pStrides        Array of strides.
   * @param pOffsets        Array of offsets.
   */
  void
  IASetVertexBuffers(unsigned int StartSlot,
                     unsigned int NumBuffers,
                     ID3D11Buffer *const *ppVertexBuffers,
                     const unsigned int* pStrides,
                     const unsigned int* pOffsets);

  /**
   * @brief Binds an Index Buffer to the Input Assembler stage.
   *
   * @param pIndexBuffer Pointer to the Index Buffer.
   * @param Format       Index format (e.g., DXGI_FORMAT_R16_UINT).
   * @param Offset       Byte offset.
   */
  void
  IASetIndexBuffer(ID3D11Buffer *pIndexBuffer,
                   DXGI_FORMAT Format,
                   unsigned int Offset);

  /**
   * @brief Sets Sampler States for the Pixel Shader stage.
   *
   * @param StartSlot   Start slot index.
   * @param NumSamplers Number of samplers.
   * @param ppSamplers  Array of Sampler State pointers.
   */
  void
  PSSetSamplers(unsigned int StartSlot,
                unsigned int NumSamplers,
                ID3D11SamplerState* const* ppSamplers);

  /**
   * @brief Sets the Rasterizer State.
   *
   * @param pRasterizerState Pointer to Rasterizer State.
   */
  void
  RSSetState(ID3D11RasterizerState* pRasterizerState);

  /**
   * @brief Sets the Blend State in the Output Merger stage.
   *
   * @param pBlendState Blend State object.
   * @param BlendFactor Array of 4 floats for blend factor.
   * @param SampleMask  Multisample mask.
   */
  void
  OMSetBlendState(ID3D11BlendState* pBlendState,
                  const float BlendFactor[4],
                  unsigned int SampleMask);

  /**
   * @brief Sets Render Targets and Depth Stencil View.
   *
   * @param NumViews            Number of render targets.
   * @param ppRenderTargetViews Array of RTV pointers.
   * @param pDepthStencilView   Pointer to DSV.
   */
  void
  OMSetRenderTargets(unsigned int NumViews,
                     ID3D11RenderTargetView* const* ppRenderTargetViews,
                     ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Sets the Primitive Topology.
   *
   * @param Topology Primitive topology type (e.g., TRIANGLELIST).
   */
  void
  IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  /**
   * @brief Clears a Render Target View with a solid color.
   *
   * @param pRenderTargetView View to clear.
   * @param ColorRGBA         Array of 4 floats (RGBA).
   */
  void
  ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
                        const float ColorRGBA[4]);

  /**
   * @brief Clears a Depth Stencil View.
   *
   * @param pDepthStencilView View to clear.
   * @param ClearFlags        Flags (Depth, Stencil, or both).
   * @param Depth             Depth value to clear to.
   * @param Stencil           Stencil value to clear to.
   */
  void
  ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                        unsigned int ClearFlags,
                        float Depth,
                        UINT8 Stencil);

  /**
   * @brief Sets Constant Buffers for the Vertex Shader stage.
   *
   * @param StartSlot   Start slot index.
   * @param NumBuffers  Number of buffers.
   * @param ppCBs       Array of Constant Buffer pointers.
   */
  void
  VSSetConstantBuffers(unsigned int StartSlot,
                       unsigned int NumBuffers,
                       ID3D11Buffer* const* ppCBs);

  /**
   * @brief Sets Constant Buffers for the Pixel Shader stage.
   *
   * @param StartSlot   Start slot index.
   * @param NumBuffers  Number of buffers.
   * @param ppCBs       Array of Constant Buffer pointers.
   */
  void
  PSSetConstantBuffers(unsigned int StartSlot,
                       unsigned int NumBuffers,
                       ID3D11Buffer* const* ppCBs);

  /**
   * @brief Draws indexed primitives.
   *
   * @param IndexCount         Number of indices to draw.
   * @param StartIndexLocation Start index in the buffer.
   * @param BaseVertexLocation Offset added to each index.
   */
  void
  DrawIndexed(unsigned int IndexCount,
              unsigned int StartIndexLocation,
              int BaseVertexLocation);
public:
  ID3D11DeviceContext* m_deviceContext = nullptr; ///< The D3D11 context.
};