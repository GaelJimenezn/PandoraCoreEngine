#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h" // Incluido para acceso a datos de vértices/índices

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Manages a generic Direct3D 11 buffer resource (Vertex, Index,
 * Constant).
 * @details Encapsulates an ID3D11Buffer, handling its creation, updating,
 * binding to the pipeline, and destruction. Can represent different buffer
 * types based on the bind flags provided during initialization.
 */
class
Buffer {
public:
  /**
   * @brief Default constructor.
   */
  Buffer() = default;

  /**
   * @brief Default destructor.
   */
  ~Buffer() = default;

  /**
   * @brief Initializes the buffer as a Vertex or Index buffer using
   * MeshComponent data.
   * @param device The graphics device for resource creation.
   * @param mesh The MeshComponent containing vertex or index data.
   * @param bindFlag Specifies the buffer type (D3D11_BIND_VERTEX_BUFFER or
   * D3D11_BIND_INDEX_BUFFER).
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

  /**
   * @brief Initializes the buffer as a Constant Buffer with a specific size.
   * @param device The graphics device for resource creation.
   * @param ByteWidth The size of the constant buffer in bytes.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device, unsigned int ByteWidth);

  /**
   * @brief Updates the content of the buffer (typically a Constant Buffer).
   * @param deviceContext The device context for command submission.
   * @param pDstResource Destination resource (usually the buffer itself).
   * @param DstSubresource Subresource index (usually 0).
   * @param pDstBox Optional box defining the region to update.
   * @param pSrcData Pointer to the data to copy into the buffer.
   * @param SrcRowPitch Row pitch of the source data (usually 0 for buffers).
   * @param SrcDepthPitch Depth pitch of the source data (usually 0 for
   * buffers).
   */
  void
  update(DeviceContext& deviceContext,
         ID3D11Resource* pDstResource,
         unsigned int    DstSubresource,
         const D3D11_BOX* pDstBox,
         const void* pSrcData,
         unsigned int    SrcRowPitch,
         unsigned int    SrcDepthPitch);

  /**
   * @brief Binds the buffer to the appropriate pipeline stage.
   * @details Binds as Vertex Buffer (IASetVertexBuffers), Index Buffer
   * (IASetIndexBuffer), or Constant Buffer (VSSetConstantBuffers
   * and optionally PSSetConstantBuffers).
   * @param deviceContext The device context for command submission.
   * @param StartSlot The starting slot index for binding.
   * @param NumBuffers The number of buffers to bind (usually 1).
   * @param setPixelShader If true and it's a Constant Buffer, also binds to
   * the Pixel Shader stage.
   * @param format Required for Index Buffers (e.g., DXGI_FORMAT_R32_UINT).
   * Ignored otherwise.
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int   StartSlot,
         unsigned int   NumBuffers,
         bool           setPixelShader = false,
         DXGI_FORMAT    format = DXGI_FORMAT_UNKNOWN);

  /**
   * @brief Releases the underlying ID3D11Buffer resource.
   */
  void
  destroy();

private:
  /**
   * @brief Helper function to create the ID3D11Buffer.
   * @param device The graphics device.
   * @param desc The buffer description structure.
   * @param initData Optional initial data for the buffer.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  createBuffer(Device& device,
               D3D11_BUFFER_DESC& desc,
               D3D11_SUBRESOURCE_DATA* initData);

private:
  /**
   * @brief The Direct3D 11 buffer object.
   */
  ID3D11Buffer* m_buffer = nullptr;

  /**
   * @brief The stride (size in bytes) of a single element (e.g., vertex
   * size or index size). For constant buffers, this might store
   * the total buffer size.
   */
  unsigned int m_stride = 0;

  /**
   * @brief The offset (in bytes) from the start of the buffer for rendering.
   * Usually 0.
   */
  unsigned int m_offset = 0;

  /**
   * @brief The bind flag(s) used during buffer creation (e.g.,
   * D3D11_BIND_VERTEX_BUFFER). Determines how the buffer is used
   * and bound in the 'render' method.
   */
  unsigned int m_bindFlag = 0;
};