#pragma once
#include "Prerequisites.h"
//#include "ECS\Component.h" // Assuming Component system integration is planned

class DeviceContext;

/**
 * @class MeshComponent
 * @brief Holds vertex and index data defining a 3D mesh.
 * @details Stores geometry information, including vertex positions, texture
 * coordinates, and the index list defining triangles or other primitives.
 * Intended to be part of a component-based system.
 */
class
MeshComponent /*: public Component // Potential inheritance */ {
public:
  /**
   * @brief Constructor, initializes vertex and index counts to zero.
   */
  MeshComponent() : m_numVertex(0), m_numIndex(0)/*, 
    Component(ComponentType::MESH)*/ {}

  /**
   * @brief Default virtual destructor.
   */
  virtual
  ~MeshComponent() = default;

  /**
   * @brief Placeholder for mesh-specific initialization 
   (e.g., loading from file).
   */
  void
  init();

  /**
   * @brief Placeholder for mesh-specific per-frame updates (e.g., animation).
   * @param deltaTime Time elapsed since the last frame.
   */
  void
  update(float deltaTime);

  /**
   * @brief Placeholder for mesh-specific rendering commands 
   *(often handled by a Renderer).
   * @param deviceContext The device context.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Placeholder for mesh resource cleanup.
   */
  void
  destroy();

public:
  /**
   * @brief Optional name for identifying the mesh.
   */
  std::string m_name;

  /**
   * @brief Vector containing the vertex data (position, texcoord, etc.).
   */
  std::vector<SimpleVertex> m_vertex;

  /**
   * @brief Vector containing the indices that define the mesh primitives 
   *(e.g., triangles).
   */
  std::vector<unsigned int> m_index;

  /**
   * @brief The total number of vertices in the mesh.
   */
  int m_numVertex;

  /**
   * @brief The total number of indices in the mesh.
   */
  int m_numIndex;
};