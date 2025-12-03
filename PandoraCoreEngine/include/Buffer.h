#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Encapsulates a D3D11 Buffer for vertices, indices, or constants.
 *
 * Manages the lifecycle and binding of an ID3D11Buffer. It supports creation
 * from MeshComponents (Vertex/Index buffers) or raw byte sizes (Constant
 * buffers), as well as updating and binding to the pipeline.
 *
 * @note Wraps a single ID3D11Buffer. The effective type is determined by
 * the internal bind flag.
 */
class
Buffer {
public:
  /**
   * @brief Default constructor.
   */
  Buffer()  = default;

  /**
   * @brief Default destructor.
   * @details Does not automatically release COM resources; call destroy().
   */
  ~Buffer() = default;

  /**
   * @brief Initializes the buffer from a MeshComponent.
   *
   * Creates an ID3D11Buffer containing vertex or index data based on the
   * bindFlag.
   *
   * @param device   The device used to create the resource.
   * @param mesh     Source mesh data.
   * @param bindFlag Bind flag (e.g., D3D11_BIND_VERTEX_BUFFER).
   * @return S_OK if successful, otherwise an HRESULT error code.
   */
  HRESULT
  init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

  /**
   * @brief Initializes the buffer as a Constant Buffer.
   *
   * Creates a buffer with D3D11_BIND_CONSTANT_BUFFER and the specified size.
   *
   * @param device    The device used to create the resource.
   * @param ByteWidth Size of the buffer in bytes (must be multiple of 16).
   * @return S_OK if successful.
   */
  HRESULT
  init(Device& device, unsigned int ByteWidth);

  /**
   * @brief Updates the buffer content.
   *
   * Uses UpdateSubresource to upload new data to the GPU.
   *
   * @param deviceContext  Context used for the update operation.
   * @param pDstResource   Destination resource (usually m_buffer).
   * @param DstSubresource Destination subresource index.
   * @param pDstBox        Destination box (nullptr for whole buffer).
   * @param pSrcData       Pointer to source data.
   * @param SrcRowPitch    Row pitch (ignored for buffers).
   * @param SrcDepthPitch  Depth pitch (ignored for buffers).
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
   * @brief Binds the buffer to the pipeline.
   *
   * Calls IASetVertexBuffers, IASetIndexBuffer, or VS/PSSetConstantBuffers
   * based on the buffer type.
   *
   * @param deviceContext  Context used for binding.
   * @param StartSlot      Start slot for binding.
   * @param NumBuffers     Number of buffers (typically 1).
   * @param setPixelShader If true and CB, binds to PS stage as well.
   * @param format         Index format (for Index Buffers only).
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int   StartSlot,
         unsigned int   NumBuffers,
         bool           setPixelShader = false,
         DXGI_FORMAT    format = DXGI_FORMAT_UNKNOWN);

  /**
   * @brief Releases the ID3D11Buffer and resets internal state.
   */
  void
  destroy();

  /**
   * @brief Helper to create a generic buffer.
   *
   * @param device   The device used for creation.
   * @param desc     Buffer description structure.
   * @param initData Initial data (optional).
   * @return S_OK if successful.
   */
  HRESULT
  createBuffer(Device& device,
               D3D11_BUFFER_DESC& desc,
               D3D11_SUBRESOURCE_DATA* initData);

private:
  ID3D11Buffer* m_buffer = nullptr; ///< The D3D11 buffer resource.
  unsigned int m_stride = 0;        ///< Element size (Vertex Buffer).
  unsigned int m_offset = 0;        ///< Offset (Vertex Buffer).
  unsigned int m_bindFlag = 0;      ///< Usage flag (e.g., Vertex, Index).
};