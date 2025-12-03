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
// Inclusión necesaria para usar Actores
#include "ECS/Actor.h" 

class 
BaseApp {
public:
	BaseApp() = default;
	~BaseApp() { destroy(); }

	int 
	run(HINSTANCE hInst, int nCmdShow);
	
	HRESULT
	init();

	void 
	update(float deltaTime);

	void 
	render();

	void 
	destroy();

private:
	static LRESULT CALLBACK 
	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

	// --- RECURSOS MOVIDOS A ACTORES ---
	// Ya no necesitamos buffers individuales aquí porque cada Actor tiene los suyos
	// Buffer							m_vertexBuffer;
	// Buffer							m_indexBuffer;
	// Texture 							m_textureCube;
	// SamplerState						m_samplerState;
	// ----------------------------------

	Buffer								m_cbNeverChanges;
	Buffer								m_cbChangeOnResize;
	
	// La matriz de proyección y vista se quedan (cámara), pero la de Mundo se va al Actor
	// XMMATRIX                         m_World; 
	XMMATRIX                            m_View;
	XMMATRIX                            m_Projection;

	Model3D*							m_model;

	// --- SISTEMA DE ACTORES ---
	std::vector<EU::TSharedPointer<Actor>> m_actors;
	EU::TSharedPointer<Actor>              m_nissanActor; // Tu actor principal
	// --------------------------

	CBChangeOnResize					cbChangesOnResize;
	CBNeverChanges						cbNeverChanges;
};