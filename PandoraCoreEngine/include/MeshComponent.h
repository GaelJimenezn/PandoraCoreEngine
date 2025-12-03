#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h" // Descomentado para incluir la clase base
class DeviceContext;

/**
 * @class MeshComponent
 * @brief Componente ECS que almacena la información de geometría (malla) de un actor.
 */
class 
MeshComponent : public Component { // Descomentado herencia
public:
  /**
   * @brief Constructor por defecto.
   */
  // Inicializa el componente y llama al constructor padre con ComponentType::MESH
  MeshComponent() : m_numVertex(0), m_numIndex(0), Component(ComponentType::MESH) {}

  virtual 
  ~MeshComponent() = default;

  void 
  init() override {}; // Override activado

  void 
  update(float deltaTime) override {}; // Override activado

  void 
  render(DeviceContext& deviceContext) override {}; // Override activado

  void
  destroy() override {}; // Override activado

public:
  std::string m_name;
  std::vector<SimpleVertex> m_vertex;
  std::vector<unsigned int> m_index;
  int m_numVertex;
  int m_numIndex;
};