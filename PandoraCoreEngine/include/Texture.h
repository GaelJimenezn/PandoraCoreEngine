#pragma once
#include "Prerequisites.h"

class 
Device;
class 
DeviceContext;

/**
 * @class Texture
 * @brief Manages a 2D texture resource and its shader resource view.
 * @details This class can handle textures loaded from files or created
 * procedurally in memory for use as render targets or depth buffers.
 */
class
Texture {
public:
  Texture() = default;
  ~Texture() = default;

  /**
   * @brief Initializes a texture by loading it from an image file.
   * @param device The device for resource creation.
   * @param textureName The file path of the texture to load.
   * @param extensionType The file format (DDS, PNG, JPG).
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device,
       const std::string& textureName,
       ExtensionType extensionType);

  /**
   * @brief Initializes an empty texture resource from memory.
   * @details Useful for creating render targets or depth stencil buffers.
   * @param device The device for resource creation.
   * @param width The width of the texture.
   * @param height The height of the texture.
   * @param Format The texture format.
   * @param BindFlags Flags indicating how the texture will be used.
   * @param sampleCount The MSAA sample count.
   * @param qualityLevels The MSAA quality level.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device,
       unsigned int width,
       unsigned int height,
       DXGI_FORMAT Format,
       unsigned int BindFlags,
       unsigned int sampleCount = 1,
       unsigned int qualityLevels = 0);

  /**
   * @brief Initializes a shader resource view from an existing texture.
   * @param device The device for resource creation.
   * @param textureRef A reference to an existing texture resource.
   * @param format The format of the shader resource view.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device, Texture& textureRef, DXGI_FORMAT format);

  /**
   * @brief Placeholder for dynamic texture data updates.
   */
  void
  update();

  /**
   * @brief Binds the texture's SRV to the pixel shader stage.
   * @param deviceContext The device context for rendering commands.
   * @param StartSlot The starting shader slot to bind the resource to.
   * @param NumViews The number of views to bind.
   */
  void
  render(DeviceContext& deviceContext,
         unsigned int StartSlot,
         unsigned int NumViews);

  /**
   * @brief Releases the texture resource and shader resource view.
   */
  void
  destroy();

public:
  /**
   * @brief The underlying GPU texture resource.
   * @details This is the actual texture data stored in video memory.
   */
  ID3D11Texture2D* m_texture = nullptr;

  /**
   * @brief The shader resource view (SRV) for this texture.
   * @details This view allows shaders to read from the texture resource.
   */
  ID3D11ShaderResourceView* m_textureFromImg = nullptr;

  /**
   * @brief The original file name or identifier for the texture.
   */
  std::string m_textureName;
};