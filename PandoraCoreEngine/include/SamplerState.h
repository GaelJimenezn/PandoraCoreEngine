#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class SamplerState
 * @brief Encapsulates an ID3D11SamplerState for texture sampling operations.
 *
 * Defines how textures are read by shaders, including filtering methods
 * (point, linear, anisotropic), addressing modes (wrap, clamp, mirror), and
 * mipmap level of detail settings.
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
   * @details Does not automatically release COM resources; call destroy().
   */
  ~SamplerState() = default;

  /**
   * @brief Initializes the Sampler State with default settings.
   *
   * Creates an ID3D11SamplerState. Typical defaults include linear filtering,
   * UV wrapping, and full mipmap range.
   *
   * @param device The device used to create the state resource.
   * @return S_OK if successful, otherwise an HRESULT error code.
   */
  HRESULT
  init(Device& device);

  /**
   * @brief Placeholder for state updates.
   *
   * Intended for dynamic reconfiguration (e.g., switching filter modes).
   */
  void
  update();

  /**
   * @brief Binds the Sampler State to the Pixel Shader stage.
   *
   * Calls PSSetSamplers to make this sampler available to shaders.
   *
   * @param deviceContext Context used for binding.
   * @param StartSlot     The starting slot index for binding.
   * @param NumSamplers   Number of samplers to bind (typically 1).
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int StartSlot,
         unsigned int NumSamplers);

  /**
   * @brief Releases the ID3D11SamplerState resource.
   *
   * Safe to call multiple times.
   */
  void
  destroy();

public:
  /**
   * @brief Pointer to the D3D11 Sampler State interface.
   * @details Valid after successful init(); nullptr after destroy().
   */
  ID3D11SamplerState* m_sampler = nullptr;
};