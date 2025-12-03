#pragma once
#include "Prerequisites.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "Component.h"

/**
 * @class Transform
 * @brief Manages position, rotation, and scale in 3D space.
 *
 * Maintains local transformation data and computes the World Matrix used
 * for rendering and physics calculations.
 */
class
Transform : public Component {
public:
  /**
   * @brief Constructor.
   *
   * Initializes identity transform. Sets type to TRANSFORM.
   */
  Transform() : position(),
                rotation(),
                scale(),
                matrix(),
                Component(ComponentType::TRANSFORM) {}

  /**
   * @brief Initializes the transform.
   *
   * Resets scale to one and matrix to identity.
   */
  void
  init() {
    scale.one();
    matrix = XMMatrixIdentity();
  }

  /**
   * @brief Updates the transformation matrix.
   *
   * Constructs the world matrix using SRT (Scale, Rotate, Translate) order.
   * @param deltaTime Time elapsed (unused).
   */
  void
  update(float deltaTime) override {
    // Apply scaling
    XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
    // Apply rotation (Roll, Pitch, Yaw)
    XMMATRIX rotationMatrix =
      XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    // Apply translation
    XMMATRIX translationMatrix =
      XMMatrixTranslation(position.x, position.y, position.z);

    // Compose final world matrix
    matrix = scaleMatrix * rotationMatrix * translationMatrix;
  }

  /**
   * @brief Render placeholder.
   * @param deviceContext Device context.
   */
  void
  render(DeviceContext& deviceContext) override {}

  /**
   * @brief Resource cleanup.
   */
  void
  destroy() {}

  /**
   * @brief Gets current position.
   * @return Const reference to position vector.
   */
  const EU::Vector3&
  getPosition() const { return position; }

  /**
   * @brief Sets position.
   * @param newPos New position vector.
   */
  void
  setPosition(const EU::Vector3& newPos) { position = newPos; }

  /**
   * @brief Gets current rotation.
   * @return Const reference to rotation vector (Euler angles).
   */
  const EU::Vector3&
  getRotation() const { return rotation; }

  /**
   * @brief Sets rotation.
   * @param newRot New rotation vector in Euler angles.
   */
  void
  setRotation(const EU::Vector3& newRot) { rotation = newRot; }

  /**
   * @brief Gets current scale.
   * @return Const reference to scale vector.
   */
  const EU::Vector3&
  getScale() const { return scale; }

  /**
   * @brief Sets scale.
   * @param newScale New scale vector.
   */
  void
  setScale(const EU::Vector3& newScale) { scale = newScale; }

  /**
   * @brief Sets position, rotation, and scale simultaneously.
   * @param newPos New position.
   * @param newRot New rotation.
   * @param newSca New scale.
   */
  void
  setTransform(const EU::Vector3& newPos,
               const EU::Vector3& newRot,
               const EU::Vector3& newSca) {
    position = newPos;
    rotation = newRot;
    scale = newSca;
  }

  /**
   * @brief Translates the object by a delta vector.
   * @param translation Vector to add to current position.
   */
  void
  translate(const EU::Vector3& translation);

private:
  EU::Vector3 position;  ///< Local position.
  EU::Vector3 rotation;  ///< Local rotation (Euler).
  EU::Vector3 scale;     ///< Local scale.

public:
  XMMATRIX matrix;       ///< Computed World Matrix.
};