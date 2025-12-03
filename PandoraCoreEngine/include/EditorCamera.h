#pragma once
#include "Prerequisites.h"
#include <cmath>
#include <algorithm>

// Asegúrate de que en Prerequisites o aquí estén definidos:
// using namespace DirectX; 

class EditorCamera {
public:
	EditorCamera() = default;
	~EditorCamera() = default;

	void init(EU::Vector3 startPos, EU::Vector3 targetPos);
	void update(float deltaTime);

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

	// Configuración de velocidades
	float m_speedOrbit = 0.005f; // Sensibilidad rotación
	float m_speedZoom = 0.5f;    // Sensibilidad rueda
	float m_speedPan = 0.01f;    // Sensibilidad movimiento lateral
};