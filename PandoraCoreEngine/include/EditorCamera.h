#pragma once
#include "Prerequisites.h"
#include <cmath>
#include <algorithm>

// Ensure DirectX namespace usage or types are defined in Prerequisites.

/**
 * @class EditorCamera
 * @brief Orbital camera system designed for scene inspection and editing.
 *
 * Implements a camera control scheme commonly found in 3D editors (Maya/Unity
 * style). It orbits around a focal point, supports zooming via distance
 * adjustment, and panning to shift the focal point.
 */
class
EditorCamera {
public:
  /**
   * @brief Default constructor.
   */
  EditorCamera() = default;

  /**
   * @brief Default destructor.
   */
  ~EditorCamera() = default;

  /**
   * @brief Initializes the camera state.
   *
   * Sets the initial position and the target point to look at.
   *
   * @param startPos  Initial world space position of the camera.
   * @param targetPos The point in world space the camera focuses on.
   */
  void
  init(EU::Vector3 startPos, EU::Vector3 targetPos);

  /**
   * @brief Updates the camera logic.
   *
   * Processes input and recalculates the view matrix based on orbital
   * movement, zooming, and panning.
   *
   * @param deltaTime Time elapsed since the last frame (in seconds).
   */
  void
  update(float deltaTime);

  /**
   * @brief Retrieves the calculated View Matrix.
   * @return The XMMATRIX transforming world space to camera space.
   */
  XMMATRIX
  getViewMatrix() const { return m_viewMatrix; }

  /**
   * @brief Retrieves the current camera position.
   * @return The position vector in world space.
   */
  XMVECTOR
  getPosition() const { return m_currentPos; }

private:
  /**
   * @brief Handles mouse and keyboard input for camera manipulation.
   *
   * Reads input devices to adjust pitch, yaw, distance (zoom), and focal
   * point (pan).
   *
   * @param deltaTime Time elapsed for frame-rate independent movement.
   */
  void
  handleInput(float deltaTime);

  /**
   * @brief Recomputes the View Matrix.
   *
   * Uses the current orbital parameters (pitch, yaw, distance) relative to
   * the focal point to derive the new view transformation.
   */
  void
  updateViewMatrix();

private:
  XMMATRIX m_viewMatrix;   ///< The current view transformation matrix.
  XMVECTOR m_currentPos;   ///< The current position in world space.
	
  // Orbit parameters
  EU::Vector3 m_focalPoint = { 0.0f, 0.0f, 0.0f }; ///< The point to look at.
  float m_distance = 10.0f;     ///< Distance from the focal point.
	
  float m_pitch = 0.0f;         ///< Vertical rotation angle (radians).
  float m_yaw = 0.0f;           ///< Horizontal rotation angle (radians).

  // Control sensitivities
  float m_speedOrbit = 0.005f;  ///< Sensitivity for rotation.
  float m_speedZoom = 0.5f;     ///< Sensitivity for scroll wheel zooming.
  float m_speedPan = 0.01f;     ///< Sensitivity for panning movements.
};