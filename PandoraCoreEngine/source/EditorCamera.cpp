#include "EditorCamera.h"
#include "ImGui/imgui.h"
#include <algorithm> // Para std::clamp

void 
EditorCamera::init(EU::Vector3 startPos, EU::Vector3 targetPos) {
	m_focalPoint = targetPos;

	// Calcular vector inicial
	float dx = startPos.x - targetPos.x;
	float dy = startPos.y - targetPos.y;
	float dz = startPos.z - targetPos.z;

	m_distance = std::sqrt(dx * dx + dy * dy + dz * dz);
	if (m_distance < 0.1f) m_distance = 10.0f;

	m_pitch = std::asin(dy / m_distance);
	m_yaw = std::atan2(dx, dz);

	updateViewMatrix();
}

void 
EditorCamera::update(float deltaTime) {
	handleInput(deltaTime);
	updateViewMatrix();
}

void 
EditorCamera::handleInput(float deltaTime) {
	ImGuiIO& io = ImGui::GetIO();

	// 1. ZOOM CON RUEDA
	if (io.MouseWheel != 0.0f) {
		float speed = m_speedZoom;
		if (io.KeyShift) speed *= 2.0f; 

		m_distance -= io.MouseWheel * speed;
		if (m_distance < 0.1f) m_distance = 0.1f;
	}

	// 2. ORBITAR (Click Derecho)
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		float dx = io.MouseDelta.x;
		float dy = io.MouseDelta.y;

		// ZOOM MANUAL (ALT + CLICK DERECHO)
		if (io.KeyAlt) {
			m_distance += dy * 0.1f;
			if (m_distance < 0.1f) m_distance = 0.1f;
		}
		else {
			// ROTACION NORMAL
			m_yaw += dx * m_speedOrbit;
			m_pitch -= dy * m_speedOrbit;
			m_pitch = std::clamp(m_pitch, -1.56f, 1.56f); // Evitar voltereta
		}
	}

	// 3. PANEO (Click Central)
	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
		float dx = io.MouseDelta.x * m_speedPan;
		float dy = io.MouseDelta.y * m_speedPan;

		XMVECTOR forward = XMVector3Normalize(m_currentPos - XMVectorSet(m_focalPoint.x, m_focalPoint.y, m_focalPoint.z, 1.0f));
		XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), forward));
		XMVECTOR up = XMVector3Normalize(XMVector3Cross(forward, right));

		XMVECTOR focusV = XMVectorSet(m_focalPoint.x, m_focalPoint.y, m_focalPoint.z, 1.0f);
		focusV += right * dx; 
		focusV += up * dy;

		XMFLOAT3 newFocus;
		XMStoreFloat3(&newFocus, focusV);
		m_focalPoint = { newFocus.x, newFocus.y, newFocus.z };
	}
}

void 
EditorCamera::updateViewMatrix() {
	// Polares a Cartesianas
	float x = m_distance * std::cos(m_pitch) * std::sin(m_yaw);
	float y = m_distance * std::sin(m_pitch);
	float z = m_distance * std::cos(m_pitch) * std::cos(m_yaw);

	XMVECTOR position = XMVectorSet(m_focalPoint.x + x, m_focalPoint.y + y, m_focalPoint.z + z, 1.0f);
	XMVECTOR focus = XMVectorSet(m_focalPoint.x, m_focalPoint.y, m_focalPoint.z, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_currentPos = position;
	m_viewMatrix = XMMatrixLookAtLH(position, focus, up);
}