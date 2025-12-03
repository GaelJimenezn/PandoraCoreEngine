#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"

class Device;
class DeviceContext;

/**
 * @class ShaderProgram
 * @brief Manages the lifecycle of Vertex and Pixel shaders.
 *
 * Handles the compilation, creation, and binding of shader programs. It also
 * manages the Input Layout associated with the Vertex Shader to ensure correct
 * data mapping from vertex buffers.
 */
class
ShaderProgram {
public:
  /**
   * @brief Default constructor.
   */
  ShaderProgram() = default;

  /**
   * @brief Default destructor.
   * @details Does not automatically release resources; call destroy().
   */
  ~ShaderProgram() = default;

  /**
   * @brief Initializes shaders from an HLSL file.
   *
   * Compiles and creates both Vertex and Pixel shaders from the specified
   * file. Also generates the Input Layout based on the provided description.
   *
   * @param device   Device used for resource creation.
   * @param fileName Path to the HLSL source file.
   * @param Layout   Vector describing the vertex input elements.
   * @return S_OK if successful.
   */
  HRESULT
  init(Device& device,
       const std::string& fileName,
       std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

  /**
   * @brief Placeholder for shader updates (e.g., hot-reloading).
   */
  void
  update();

  /**
   * @brief Binds the Vertex Shader, Pixel Shader, and Input Layout.
   *
   * Sets the active pipeline state for rendering.
   *
   * @param deviceContext Context used for binding.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Binds a specific shader stage only.
   *
   * @param deviceContext Context used for binding.
   * @param type          The shader stage to bind (VS or PS).
   */
  void
  render(DeviceContext& deviceContext, ShaderType type);

  /**
   * @brief Releases all shader resources and blobs.
   */
  void
  destroy();

  /**
   * @brief Creates the Input Layout for the Vertex Shader.
   *
   * @param device Device used for creation.
   * @param Layout Vertex element description vector.
   * @return S_OK if successful.
   */
  HRESULT
  CreateInputLayout(Device& device,
                    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

  /**
   * @brief Creates a shader using the internal filename.
   *
   * @param device Device used for creation.
   * @param type   Shader type (VS or PS).
   * @return S_OK if successful.
   */
  HRESULT
  CreateShader(Device& device, ShaderType type);

  /**
   * @brief Creates a shader from a specified file.
   *
   * @param device   Device used for creation.
   * @param type     Shader type (VS or PS).
   * @param fileName Path to the HLSL file.
   * @return S_OK if successful.
   */
  HRESULT
  CreateShader(Device& device, ShaderType type, const std::string& fileName);

  /**
   * @brief Compiles HLSL code from a file.
   *
   * Uses D3DCompileFromFile to generate shader bytecode.
   *
   * @param szFileName    File path.
   * @param szEntryPoint  Main function name (e.g., "VSMain").
   * @param szShaderModel Shader profile (e.g., "vs_5_0").
   * @param ppBlobOut     Output pointer for the compiled blob.
   * @return S_OK if successful.
   */
  HRESULT
  CompileShaderFromFile(char* szFileName,
                        LPCSTR szEntryPoint,
                        LPCSTR szShaderModel,
                        ID3DBlob** ppBlobOut);

public:
  ID3D11VertexShader* m_VertexShader = nullptr; ///< Vertex Shader resource.
  ID3D11PixelShader* m_PixelShader = nullptr;   ///< Pixel Shader resource.
  InputLayout m_inputLayout;                    ///< Associated Input Layout.

private:
  std::string m_shaderFileName;           ///< Path to the HLSL file.
  ID3DBlob* m_vertexShaderData = nullptr; ///< Compiled VS bytecode.
  ID3DBlob* m_pixelShaderData = nullptr;  ///< Compiled PS bytecode.
};