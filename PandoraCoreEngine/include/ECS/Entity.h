#pragma once
#include "Prerequisites.h"
#include "Component.h"

class DeviceContext;

/**
 * @class Entity
 * @brief Base class for generic objects within the ECS system.
 *
 * Acts as a container for Components, managing their lifecycle and allowing
 * dynamic composition of behavior.
 */
class
Entity {
public:
  /**
   * @brief Default constructor.
   */
  Entity() = default;

  /**
   * @brief Virtual destructor.
   */
  virtual
  ~Entity() = default;

  /**
   * @brief Initializes the entity.
   *
   * Triggers initialization of internal states or attached components.
   */
  virtual void
  init() = 0;

  /**
   * @brief Updates the entity and its components.
   *
   * @param deltaTime     Time elapsed since last frame (seconds).
   * @param deviceContext Context for graphical updates.
   */
  virtual void
  update(float deltaTime, DeviceContext& deviceContext) = 0;

  /**
   * @brief Renders the entity.
   * @param deviceContext Context used for rendering.
   */
  virtual void
  render(DeviceContext& deviceContext) = 0;

  /**
   * @brief Destroys the entity and releases resources.
   *
   * Iterates through components calling their destroy methods.
   */
  virtual void
  destroy() = 0;

  /**
   * @brief Attaches a component to the entity.
   * @tparam T Component type (must derive from Component).
   * @param component Shared pointer to the component instance.
   */
  template <typename T>
  void
  addComponent(EU::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be derived from Component");
    m_components.push_back(
      component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Retrieves an attached component by type.
   * @tparam T Type of component to retrieve.
   * @return Shared pointer to component if found, else nullptr.
   */
  template<typename T>
  EU::TSharedPointer<T>
  getComponent() {
    for (auto& component : m_components) {
      auto specificComponent =
        component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EU::TSharedPointer<T>();
  }

protected:
  bool m_isActive; ///< Entity active state flag.
  int m_id;        ///< Unique entity identifier.
  std::vector<EU::TSharedPointer<Component>> m_components; ///< Component list.
};