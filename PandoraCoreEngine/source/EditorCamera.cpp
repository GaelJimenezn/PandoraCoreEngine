#pragma once
#include "Prerequisites.h"
#include <cmath>

class EditorCamera {
public:
	EditorCamera() = default;
	~EditorCamera() = default;

	// Inicializa la camara en una posicion mirando al centro
	void init(EU::Vector3 startPos, EU::Vector3 targetPos);

	// Actualiza input y matrices
	void update(float deltaTime);

	// Getters
	XMMATRIX getViewMatrix() const { return m_viewMatrix; }
	XMVECTOR getPosition() const { return m_currentPos; }

private:
	void handleInput(float deltaTime);
	void updateViewMatrix();

private:
	XMMATRIX m_viewMatrix;
	XMVECTOR m_currentPos;
	
	// Orbita
	EU::Vector3 m_focalPoint = { 0.0f, 0.0f, 0.0f };
	float m_distance = 10.0f;
	
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;

	// Configuración
	float m_speedOrbit = 0.005f;
	float m_speedZoom = 2.0f;
	float m_speedPan = 0.02f;
};