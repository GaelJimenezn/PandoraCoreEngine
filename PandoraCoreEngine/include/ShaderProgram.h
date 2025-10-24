#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"

class 
Device;
class 
DeviceContext;

/**
 * @class ShaderProgram
 * @brief Manages a pair of vertex and pixel shaders and their input layout.
 * @details Compiles HLSL shaders from a file, creates the corresponding
 * shader objects (ID3D11VertexShader, ID3D11PixelShader), and sets up the
 * InputLayout based on the vertex shader's input signature.
 */
class
ShaderProgram {
public:
  /**
   * @brief Default constructor.
   */
  ShaderProgram() = default;

  /**
   * @brief Default destructor. Calls destroy().
   */
  ~ShaderProgram() = default; // Implicitly calls destroy() if needed

  /**
   * @brief Initializes the shader program by compiling shaders and creating layout.
   * @param device The graphics device used for resource creation.
   * @param fileName The base name of the HLSL shader file (expects .fx extension).
   * @param Layout A vector describing the vertex input layout elements.
   * @return HRESULT indicating success or failure.
   */
 HRESULT
 init(Device& device,
       const std::string& fileName,
       std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

 /**
  * @brief Placeholder for potential shader update logic (e.g., reloading).
  */
 void
 update();

 /**
  * @brief Binds the vertex shader, pixel shader, and input layout to the pipeline.
  * @param deviceContext The device context for command submission.
  */
 void
 render(DeviceContext& deviceContext);

 /**
  * @brief Binds a specific shader type (Vertex or Pixel) to the pipeline.
  * @param deviceContext The device context for command submission.
  * @param type The type of shader to bind (VERTEX_SHADER or PIXEL_SHADER).
  */
 void
 render(DeviceContext& deviceContext, ShaderType type);

 /**
  * @brief Releases all shader objects and associated data.
  */
 void
 destroy();

private:
 /**
  * @brief Creates the input layout object based on vertex shader data.
  * @param device The graphics device.
  * @param Layout A vector describing the vertex input layout elements.
  * @return HRESULT indicating success or failure.
  */
 HRESULT
 CreateInputLayout(Device& device,
                    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

 /**
  * @brief Creates either a vertex or pixel shader from the loaded file.
  * @param device The graphics device.
  * @param type The type of shader to create (VERTEX_SHADER or PIXEL_SHADER).
  * @return HRESULT indicating success or failure.
  */
 HRESULT
 CreateShader(Device& device, ShaderType type);

 /**
  * @brief Creates either a vertex or pixel shader from a specified file.
  * @param device The graphics device.
  * @param type The type of shader to create (VERTEX_SHADER or PIXEL_SHADER).
  * @param fileName The name of the HLSL file.
  * @return HRESULT indicating success or failure.
  */
 HRESULT
 CreateShader(Device& device, ShaderType type, const std::string& fileName);

 /**
  * @brief Compiles an HLSL shader from a file.
  * @param szFileName Path to the HLSL file.
  * @param szEntryPoint Name of the shader entry point function (e.g., "VS", "PS").
  * @param szShaderModel Shader model target (e.g., "vs_4_0", "ps_4_0").
  * @param ppBlobOut [out] Receives a pointer to the compiled shader bytecode.
  * @return HRESULT indicating success or failure.
  */
 HRESULT
 CompileShaderFromFile(char* szFileName,
                       LPCSTR szEntryPoint,
                       LPCSTR szShaderModel,
                       ID3DBlob** ppBlobOut);

public:
  /**
   * @brief The Direct3D 11 vertex shader object.
   */
  ID3D11VertexShader* m_VertexShader = nullptr;

  /**
   * @brief The Direct3D 11 pixel shader object.
   */
  ID3D11PixelShader* m_PixelShader = nullptr;

  /**
   * @brief The InputLayout object associated with the vertex shader.
   */
  InputLayout m_inputLayout;

private:
  /**
   * @brief The base filename of the shader source file.
   */
  std::string m_shaderFileName;

  /**
   * @brief Compiled vertex shader bytecode data. Released after input layout creation.
   */
  ID3DBlob* m_vertexShaderData = nullptr;

  /**
   * @brief Compiled pixel shader bytecode data. Kept for potential reflection.
   */
  ID3DBlob* m_pixelShaderData = nullptr;
};