#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class Texture
 * @brief Wraps a D3D11 Texture2D and its Shader Resource View.
 *
 * Manages textures created from files (PNG, DDS, etc.) or raw memory.
 * Provides functionality to bind textures to shaders as resources.
 */
class
Texture {
public:
  /**
   * @brief Default constructor.
   */
  Texture()  = default;

  /**
   * @brief Default destructor.
   */
  ~Texture() = default;

  /**
   * @brief Loads a texture from a file.
   *
   * Creates the texture resource and a Shader Resource View (SRV).
   *
   * @param device        Device used for creation.
   * @param textureName   File path to the image.
   * @param extensionType File extension (PNG, JPG, etc.).
   * @return S_OK if successful.
   */
  HRESULT
  init(Device & device,
       const std::string & textureName,
       ExtensionType extensionType);

  /**
   * @brief Creates a texture from memory specifications.
   *
   * Useful for Render Targets, Depth Buffers, or procedural textures.
   *
   * @param device        Device used for creation.
   * @param width         Texture width.
   * @param height        Texture height.
   * @param Format        DXGI Format (e.g., R8G8B8A8_UNORM).
   * @param BindFlags     Usage flags (e.g., SHADER_RESOURCE | RENDER_TARGET).
   * @param sampleCount   MSAA sample count (default 1).
   * @param qualityLevels MSAA quality levels.
   * @return S_OK if successful.
   */
  HRESULT
  init(Device & device,
       unsigned int width,
       unsigned int height,
       DXGI_FORMAT Format,
       unsigned int BindFlags,
       unsigned int sampleCount = 1,
       unsigned int qualityLevels = 0);

  /**
   * @brief Creates a texture based on another texture's properties.
   *
   * @param device     Device used for creation.
   * @param textureRef Reference texture to copy properties from.
   * @param format     New format for the created texture.
   * @return S_OK if successful.
   */
  HRESULT
  init(Device& device, Texture& textureRef, DXGI_FORMAT format);

  /**
   * @brief Placeholder for dynamic texture updates.
   */
  void
  update();

  /**
   * @brief Binds the texture to the Pixel Shader stage.
   *
   * @param deviceContext Context used for binding.
   * @param StartSlot     Slot index for the resource.
   * @param NumViews      Number of views to bind.
   */
  void
  render(DeviceContext & deviceContext,
         unsigned int StartSlot,
         unsigned int NumViews);

  /**
   * @brief Releases texture resources and views.
   */
  void
  destroy();

public:
  ID3D11Texture2D* m_texture = nullptr; ///< Raw D3D11 Texture resource.
  ID3D11ShaderResourceView* m_textureFromImg = nullptr; ///< SRV interface.
  std::string m_textureName; ///< Texture identifier/filename.
};