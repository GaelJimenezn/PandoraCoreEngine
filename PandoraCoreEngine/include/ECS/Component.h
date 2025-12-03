#pragma once
#include "Prerequisites.h"
class DeviceContext;

/**
 * @class Component
 * @brief Abstract base class for all ECS functional components.
 *
 * Defines the interface for lifecycle management (init, update, render,
 * destroy) that all components must implement.
 */
class
Component {
public:
  /**
   * @brief Default constructor.
   */
  Component() = default;

  /**
   * @brief Constructs a component with a specific type.
   * @param type The enumerated type identifier.
   */
  Component(const ComponentType type) : m_type(type) {}

  /**
   * @brief Virtual destructor.
   */
  virtual
  ~Component() = default;

  /**
   * @brief Initializes the component.
   *
   * Pure virtual. Implementations handle allocation and setup here.
   */
  virtual void
  init() = 0;

  /**
   * @brief Updates component logic.
   * @param deltaTime Time elapsed since the last frame (seconds).
   */
  virtual void
  update(float deltaTime) = 0;

  /**
   * @brief Renders the component.
   * @param deviceContext Context for graphics commands.
   */
  virtual void
  render(DeviceContext& deviceContext) = 0;

  /**
   * @brief Destroys the component.
   *
   * Pure virtual. Implementations must release resources here.
   */
  virtual void
  destroy() = 0;

  /**
   * @brief Retrieves the component type.
   * @return The ComponentType enum value.
   */
  ComponentType
  getType() const { return m_type; }

protected:
  ComponentType m_type; ///< Identifier for the component type.
};