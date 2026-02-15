#pragma once
#include "Prerequisites.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

class Viewport;
class Window;
class Device;
class DeviceContext;
class Actor;

/**
 * @class GUI
 * @brief Sistema gestor de la interfaz gráfica de usuario (ImGui).
 *
 * Administra el ciclo de vida de ImGui, la renderización de ventanas de
 * herramientas (Inspector, Outliner, Toolbar) y los gizmos de transformación.
 */
class GUI {
public:
    GUI() = default;
    ~GUI() = default;

    /**
     * @brief Configuración inicial antes de crear el contexto gráfico.
     */
    void 
    awake();

    /**
     * @brief Inicializa ImGui y sus backends (Win32 y DX11).
     * @param window Ventana principal de la aplicación.
     * @param device Dispositivo DirectX para crear recursos de fuentes/texturas.
     * @param deviceContext Contexto para comandos de dibujo.
     */
    void 
    init(Window& window, Device& device, DeviceContext& deviceContext);

    /**
     * @brief Inicia el frame de ImGui y construye la interfaz.
     *
     * Define el layout, dockspaces y ventanas visibles.
     * @param viewport Referencia al viewport de la escena.
     * @param window Referencia a la ventana del sistema.
     */
    void 
    update(Viewport& viewport, Window& window);
  
    /**
     * @brief Renderiza los datos de dibujo de ImGui.
     *
     * Se debe llamar al final del frame, antes de presentar el SwapChain.
     */
    void 
    render();
  
    /**
     * @brief Cierra ImGui y libera sus recursos.
     */
    void 
    destroy();

    /**
     * @brief Dibuja la barra de herramientas principal (Menú superior).
     */
    void 
    ToolBar();
  
    /**
     * @brief Muestra el popup de confirmación para cerrar la aplicación.
     */
    void 
    closeApp();

    /**
     * @brief Configura y muestra tooltips informativos.
     */
    void
    toolTipData();

    /**
     * @brief Aplica un estilo visual personalizado (Tema oscuro/azul).
     * @param opacity Opacidad de las ventanas (0.0f a 1.0f).
     * @param accent Color de acento principal.
     */
    void
    appleLiquidStyle(float opacity, ImVec4 accent);

    /**
     * @brief Widget personalizado para controlar vectores de 3 componentes.
     *
     * Útil para editar posición, rotación y escala en el inspector.
     * @param label Etiqueta del control (X, Y, Z).
     * @param values Puntero al array de 3 floats a modificar.
     * @param resetValues Valor al que se resetea si se hace clic derecho.
     * @param columnWidth Ancho de la etiqueta.
     */
    void
    vec3Control(const std::string& label,
                float* values,
                float resetValues = 0.0f,
                float columnWidth = 100.0f);

    /**
     * @brief Renderiza el panel Inspector para un actor.
     *
     * Muestra componentes y propiedades editables del actor seleccionado.
     * @param actor Puntero al actor seleccionado.
     */
    void
    inspectorGeneral(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Contenedor principal del inspector.
     * @param actor Actor a inspeccionar.
     */
    void
    inspectorContainer(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Muestra la lista jerárquica de actores en la escena.
     *
     * Permite seleccionar actores.
     * @param actors Lista de todos los actores en la escena.
     */
    void
    outliner(const std::vector<EU::TSharedPointer<Actor>>& actors);

    /**
     * @brief Maneja la lógica de los Gizmos de transformación (Mover/Rotar/Escalar).
     * @param view Matriz de vista de la cámara.
     * @param projection Matriz de proyección de la cámara.
     * @param actor Actor seleccionado sobre el cual aplicar el gizmo.
     */
    void 
    editTransform(const XMMATRIX& view, 
                  const XMMATRIX& projection, 
                  EU::TSharedPointer<Actor> actor);

    /**
     * @brief Dibuja la barra de herramientas para seleccionar modo de Gizmo.
     */
    void 
    drawGizmoToolbar();

    /**
     * @brief Convierte una XMMATRIX a un array de float[16].
     *
     * Necesario para compatibilidad con ImGuizmo.
     * @param mat Matriz de entrada (DirectX Math).
     * @param dest Puntero al array de destino.
     */
    void ToFloatArray(const XMMATRIX& mat, float* dest) {
        XMFLOAT4X4 temp;
        XMStoreFloat4x4(&temp, mat);
        memcpy(dest, &temp, sizeof(float) * 16);
    }

private:
    bool checkboxValue = true;
    bool checkboxValue2 = false;
    std::vector<const char*> m_objectsNames;
    std::vector<const char*> m_tooltips;

    bool show_exit_popup = false; ///< Controla la visibilidad del popup de salida.

public:
    int selectedActorIndex = -1; ///< Índice del actor seleccionado en el Outliner.
};