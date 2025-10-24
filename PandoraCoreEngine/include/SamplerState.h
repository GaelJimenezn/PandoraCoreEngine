#pragma once
#include "Device.h"
#include "DeviceContext.h"

/**
 * @class SamplerState
 * @brief Manages a Direct3D 11 sampler state object.
 * @details Encapsulates an ID3D11SamplerState, which defines how texture
 * data is sampled (filtered, addressed) within shaders.
 */
class
SamplerState {

public:
  /**
   * @brief Default constructor.
   */
  SamplerState() = default;

  /**
   * @brief Default destructor.
   */
  ~SamplerState() = default;

  /**
   * @brief Initializes the sampler state with default linear filtering and wrapping.
   * @param device The graphics device used for resource creation.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device);

  /**
   * @brief Placeholder for potential sampler state update logic.
   */
  void
  update();

  /**
   * @brief Binds the sampler state to the pixel shader stage.
   * @param deviceContext The device context for command submission.
   * @param StartSlot The starting sampler slot index.
   * @param NumSamplers The number of samplers to bind (usually 1).
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int StartSlot,
         unsigned int NumSamplers);

  /**
   * @brief Releases the underlying ID3D11SamplerState resource.
   */
  void
  destroy();

public:
  /**
   * @brief The Direct3D 11 sampler state object.
   * @details Defines filtering modes (e.g., linear, point) and
   * addressing modes (e.g., wrap, clamp) for texture sampling.
   */
  ID3D11SamplerState* m_sampler = nullptr;
};