#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"

// --- MODULOS NUEVOS ---
#include "EditorUI.h" 
#include "EditorCamera.h" // Nueva camara

class BaseApp {
public:
	BaseApp() = default;
	~BaseApp() { destroy(); }

	int run(HINSTANCE hInst, int nCmdShow);
	HRESULT init();
	void update(float deltaTime);
	void render();
	void destroy();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	// Helpers para el borde brillante
	HRESULT initOutlineStates();
	void renderOutline(int actorIndex);

private:
	Window                              m_window;
	Device								m_device;
	DeviceContext						m_deviceContext;
	SwapChain                           m_swapChain;
	Texture                             m_backBuffer;
	RenderTargetView					m_renderTargetView;
	Texture                             m_depthStencil;
	DepthStencilView					m_depthStencilView;
	Viewport                            m_viewport;
	ShaderProgram						m_shaderProgram;

	Buffer								m_cbNeverChanges;
	Buffer								m_cbChangeOnResize;
	
	// Ya no usamos m_View directamente, usamos la camara
	// XMMATRIX                         m_View; 
	XMMATRIX                            m_Projection;

	Model3D*							m_model;

	std::vector<EU::TSharedPointer<Actor>> m_actors;
	EU::TSharedPointer<Actor>              m_nissanActor;

	CBChangeOnResize					cbChangesOnResize;
	CBNeverChanges						cbNeverChanges;

	// --- EDITOR ---
	Editor::EditorUI                    m_editor;
	EditorCamera                        m_camera; // NUEVA CAMARA ORBITAL
	int                                 m_selectedActorIndex = -1;

	// Estados DX11 para Outline
	ID3D11DepthStencilState* m_stencilStateWrite = nullptr;
	ID3D11DepthStencilState* m_stencilStateMask = nullptr;
};