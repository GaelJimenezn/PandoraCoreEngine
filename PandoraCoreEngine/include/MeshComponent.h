#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"

class DeviceContext;

/**
 * @class MeshComponent
 * @brief ECS component holding geometric data for an entity.
 *
 * Stores raw vertex and index data required to render a mesh. It acts as the
 * data source for creating hardware buffers (Vertex/Index Buffer).
 */
class
MeshComponent : public Component {
public:
  /**
   * @brief Default constructor.
   *
   * Initializes counters to zero and sets the component type to MESH.
   */
  MeshComponent()
    : m_numVertex(0), m_numIndex(0), Component(ComponentType::MESH) {}

  /**
   * @brief Default destructor.
   */
  virtual
  ~MeshComponent() = default;

  /**
   * @brief Initialization override.
   *
   * Currently empty; geometry is typically loaded externally or via
   * setters.
   */
  void
  init() override {};

  /**
   * @brief Update override.
   * @param deltaTime Time elapsed (unused for static meshes).
   */
  void
  update(float deltaTime) override {};

  /**
   * @brief Render override.
   * @param deviceContext Device context (unused directly by component).
   */
  void
  render(DeviceContext& deviceContext) override {};

  /**
   * @brief Destruction override.
   *
   * Clears local vectors.
   */
  void
  destroy() override {};

public:
  std::string m_name;                 ///< Name of the mesh asset.
  std::vector<SimpleVertex> m_vertex; ///< CPU-side vertex data.
  std::vector<unsigned int> m_index;  ///< CPU-side index data.
  int m_numVertex;                    ///< Vertex count.
  int m_numIndex;                     ///< Index count.
};