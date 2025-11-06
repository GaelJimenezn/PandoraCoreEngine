#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class InputLayout
 * @brief Manages the Direct3D 11 Input Layout object.
 * @details Encapsulates an ID3D11InputLayout, which describes how vertex data
 * from input buffers is mapped to the input signature of a vertex shader.
 */
class
InputLayout {

public:
  /**
   * @brief Default constructor.
   */
  InputLayout() = default;

  /**
   * @brief Default destructor.
   */
  ~InputLayout() = default;

  /**
   * @brief Initializes the input layout object.
   * @param device The graphics device used for creation.
   * @param layout A vector of D3D11_INPUT_ELEMENT_DESC 
   * describing the vertex format.
   * @param VertexShaderData A blob containing the compiled vertex
   * shader bytecode,
   * which includes the required input signature.
   * @return HRESULT indicating success or failure.
   */
  HRESULT
  init(Device& device,
       std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
       ID3DBlob* VertexShaderData);

  /**
   * @brief Placeholder for potential update logic (rarely needed for input layouts).
   */
  void
  update();

  /**
   * @brief Binds the input layout object to the Input Assembler (IA) stage.
   * @param deviceContext The device context for command submission.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Releases the underlying ID3D11InputLayout resource.
   */
  void
  destroy();

public:
  /**
   * @brief The Direct3D 11 input layout object.
   * @details Defines the structure of vertex data expected by the vertex shader.
   */
  ID3D11InputLayout* m_inputLayout = nullptr;

};