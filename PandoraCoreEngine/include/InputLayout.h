#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class InputLayout
 * @brief Encapsulates an ID3D11InputLayout describing vertex formats.
 *
 * Defines how vertex data in memory is mapped to the input parameters of the
 * Vertex Shader. This class manages the creation and binding of the layout
 * resource to the Input Assembler stage.
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
   * @details Does not automatically release COM resources; call destroy().
   */
  ~InputLayout() = default;

  /**
   * @brief Initializes the Input Layout.
   *
   * Creates the ID3D11InputLayout using the provided element descriptions
   * and the compiled shader signature.
   *
   * @param device            Device used for resource creation.
   * @param Layout            Vector of input element descriptions.
   * @param VertexShaderData  Compiled Vertex Shader blob containing the
   * input signature.
   * @return S_OK if successful, otherwise an HRESULT error code.
   */
  HRESULT
  init(Device& device,
       std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout,
       ID3DBlob* VertexShaderData);

  /**
   * @brief Placeholder for layout updates.
   *
   * Currently performs no operations but is reserved for dynamic layout
   * modification.
   */
  void
  update();

  /**
   * @brief Binds the Input Layout to the pipeline.
   *
   * Calls IASetInputLayout on the provided device context.
   *
   * @param deviceContext The context used for binding.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Releases the ID3D11InputLayout resource.
   *
   * Resets the internal pointer to nullptr. Safe to call multiple times.
   */
  void
  destroy();

public:
  /**
   * @brief The D3D11 Input Layout interface.
   * @details Valid after successful init(); nullptr after destroy().
   */
  ID3D11InputLayout* m_inputLayout = nullptr;
};