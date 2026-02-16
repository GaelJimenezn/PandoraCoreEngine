/**
 * @file BaseApp.h
 * @brief Framework central de aplicación para el motor Pandora Core.
 *
 * Este archivo define la clase BaseApp, la cual orquesta el ciclo de vida
 * completo de la aplicación, desde la creación de la ventana hasta la
 * gestión del bucle de renderizado y la integración con DirectX 11.
 *
 * @author Gael Jimenez
 * @copyright Obsidian Node Studio
 * @version 1.0
 */

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
#include "GUI/GUI.h"
#include "SceneGraph/SceneGraph.h"
#include "EngineUtilities\Utilities\Camera.h"

/**
 * @brief Manejador externo para los mensajes de Win32 de ImGui.
 *
 * Reenvía los eventos del sistema operativo (mouse, teclado) al contexto
 * de ImGui para permitir la interacción con la interfaz de usuario.
 *
 * @param hWnd Handle de la ventana que recibe el mensaje.
 * @param msg Identificador del mensaje de Windows.
 * @param wParam Información adicional del mensaje.
 * @param lParam Información adicional del mensaje.
 * @return LRESULT Resultado del procesamiento del mensaje.
 */
extern IMGUI_IMPL_API
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, 
                                       WPARAM wParam, LPARAM lParam);

/**
 * @class BaseApp
 * @brief Clase base abstracta para aplicaciones basadas en D3D11.
 *
 * Proporciona la infraestructura necesaria para gestionar el bucle infinito,
 * el bombeo de mensajes de Windows y la inicialización de los recursos 
 * críticos de hardware como el Device, DeviceContext y el SwapChain.
 */
class
BaseApp {
public:
	/**
	 * @brief Constructor por defecto de la clase BaseApp.
	 */
	BaseApp() = default;

	/**
	 * @brief Destructor virtual.
	 * Se encarga de invocar el método destroy() para garantizar que todos
	 * los recursos de GPU y memoria dinámica se liberen correctamente.
	 */
	virtual ~BaseApp() { destroy(); }

	/**
	 * @brief Lógica de pre-inicialización del motor.
	 *
	 * Se ejecuta antes de la creación de la ventana o del contexto gráfico.
	 * Es el lugar ideal para cargar configuraciones o inicializar logs.
	 *
	 * @return HRESULT S_OK si la operación fue exitosa.
	 */
	HRESULT
	awake();

	/**
	 * @brief Inicia el punto de entrada y el bucle principal.
	 *
	 * Crea la ventana y los sistemas base, entrando luego en un ciclo que
	 * llama a update() y render() en cada fotograma hasta el cierre.
	 *
	 * @param hInst Identificador de la instancia de la aplicación.
	 * @param nCmdShow Estado de visualización inicial de la ventana.
	 * @return int Código de salida de la aplicación para el SO.
	 */
	int
	run(HINSTANCE hInst, int nCmdShow);

	/**
	 * @brief Inicializa los subsistemas centrales de DirectX 11.
	 *
	 * Crea el Device, SwapChain, RenderTargetView y DepthStencilView, 
	 * además de preparar los Shaders y Buffers del pipeline inicial.
	 *
	 * @return HRESULT S_OK en éxito, código de error en fallo.
	 */
	HRESULT
	init();

	/**
	 * @brief Actualización de la lógica del motor por frame.
	 *
	 * @param deltaTime Tiempo transcurrido (en segundos) desde el frame
	 * anterior, utilizado para cálculos de física y animaciones.
	 */
	void
	update(float deltaTime);

	/**
	 * @brief Rutina de renderizado por frame.
	 *
	 * Limpia los buffers, procesa la geometría a través del pipeline 
	 * gráfico y presenta el back buffer en pantalla.
	 */
	void
	render();

	/**
	 * @brief Renderiza la capa de interfaz de usuario gráfica.
	 */
	void 
	renderGUI();

	/**
	 * @brief Libera de forma segura todos los recursos asignados.
	 */
	void
	destroy();

private:
	/**
	 * @brief Procedimiento de Ventana (WndProc) estático.
	 *
	 * Procesa eventos del sistema (redimensión, cierre, inputs) y los
	 * distribuye tanto al motor como al manejador de ImGui.
	 */
	static LRESULT CALLBACK
		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	/** @name Miembros de Ventana y Gráficos */
	///@{
	Window           m_window;           /**< Envoltorio de la ventana. */
	Device           m_device;           /**< Interfaz del dispositivo. */
	DeviceContext    m_deviceContext;    /**< Contexto de comandos. */
	SwapChain        m_swapChain;        /**< Cadena de intercambio. */
	///@}

	/** @name Buffers y Vistas de Renderizado */
	///@{
	Texture          m_backBuffer;       /**< Textura del buffer trasero. */
	RenderTargetView m_renderTargetView; /**< Vista de destino de render. */
	Texture          m_depthStencil;     /**< Textura para depth test. */
	DepthStencilView m_depthStencilView; /**< Vista de profundidad. */
	Viewport         m_viewport;         /**< Configuración de Viewport. */
	///@}

	/** @name Shaders y Recursos de Memoria */
	///@{
	ShaderProgram    m_shaderProgram;    /**< Programa de shaders. */
	Buffer           m_cbNeverChanges;   /**< CB para datos estáticos. */
	Buffer           m_cbChangeOnResize; /**< CB para datos dinámicos. */
	Texture          m_modelAlbedo;      /**< Albedo del modelo demo. */
	Texture          m_skyboxTex;        /**< Cubemap para el cielo. */
	///@}

	/** @name Cámaras y Transformaciones */
	///@{
	Camera           m_camera;           /**< Cámara de la escena. */
	XMMATRIX         m_View;             /**< Matriz de Vista. */
	XMMATRIX         m_Projection;       /**< Matriz de Proyección. */
	///@}

	/** @name Escena y ECS */
	///@{
	SceneGraph       m_sceneGraph;       /**< Jerarquía de la escena. */
	/** @brief Lista de actores instanciados. */
	std::vector<EU::TSharedPointer<Actor>> m_actors;
	/** @brief Actor principal para la demostración. */
	EU::TSharedPointer<Actor> m_modelActor;
	/** @brief Recurso de geometría cargado. */
	Model3D* m_modelResource = nullptr;
	///@}

	/** @name Estructuras de Datos y GUI */
	///@{
	CBChangeOnResize cbChangesOnResize; /**< Estructura CPU para Resize. */
	CBNeverChanges   cbNeverChanges;    /**< Estructura CPU Constante. */
	GUI              m_gui;             /**< Gestor de ImGui. */
	///@}

	/** @brief Vistas para depurar las caras del Cubemap en ImGui. */
	ID3D11ShaderResourceView* m_faceDebugSRV[6] = { nullptr };
};