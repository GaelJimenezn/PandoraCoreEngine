#pragma once
#include "Prerequisites.h"
#include "Component.h"
#include <vector>

class DeviceContext;

/**
 * @class Entity
 * @brief Clase base abstracta que representa un objeto en el mundo del juego.
 *
 * Funciona como contenedor de componentes. Gestiona el ciclo de vida (awake,
 * init, update, render, destroy) y la propiedad de sus componentes.
 */
class Entity {
public:
    /**
     * @brief Constructor por defecto.
     */
    Entity() = default;

    /**
     * @brief Destructor virtual.
     */
    virtual ~Entity() = default;

    /**
     * @brief Fase de "Despertar" de la entidad.
     *
     * Se ejecuta antes de init(). Ideal para inicializar variables internas
     * o estados que no dependen de referencias externas.
     */
    virtual void 
    awake() = 0;

    /**
     * @brief Inicializa la entidad con un contexto de dispositivo.
     *
     * Se debe llamar después de awake(). Aquí se cargan recursos o se
     * establecen referencias dependientes del contexto.
     * @param deviceContext Contexto para inicialización gráfica/lógica.
     */
    virtual void 
    init() = 0;

    /**
     * @brief Actualiza la lógica de la entidad y sus componentes.
     *
     * Se llama una vez por frame.
     * @param deltaTime Tiempo transcurrido (segundos) desde el último frame.
     * @param deviceContext Referencia al contexto del dispositivo actual.
     */
    virtual void 
    update(float deltaTime, DeviceContext& deviceContext) = 0;

    /**
     * @brief Renderiza la entidad y sus componentes visuales.
     *
     * Se llama en la fase de dibujado del loop principal.
     * @param deviceContext Contexto para operaciones gráficas.
     */
    virtual void 
    render(DeviceContext& deviceContext) = 0;

    /**
     * @brief Destruye la entidad y libera recursos.
     *
     * Limpia memoria, libera punteros y desconecta la entidad.
     */
    virtual void 
    destroy() = 0;

    /**
     * @brief Agrega un nuevo componente a la entidad.
     *
     * @tparam T Tipo del componente (derivado de Component).
     * @param component Puntero compartido al componente a agregar.
     */
    template <typename T> void 
    addComponent(EU::TSharedPointer<T> component) {
        static_assert(std::is_base_of<Component, T>::value, 
                      "T must be derived from Component");
        m_components.push_back(
            component.template dynamic_pointer_cast<Component>());
    }

    /**
     * @brief Busca un componente específico en la entidad.
     *
     * @warning Operación O(N). Evitar en bucles críticos.
     * @tparam T Tipo del componente a obtener.
     * @return Puntero al componente si existe, o puntero vacío.
     */
    template<typename T>
    EU::TSharedPointer<T> 
    getComponent() {
        for (auto& component : m_components) {
            auto specific = component.template dynamic_pointer_cast<T>();
            if (specific) {
                return specific;
            }
        }
        return EU::TSharedPointer<T>();
    }

protected:
    /** * @brief Indica si la entidad está activa en la escena.
     * Si es false, no se actualiza ni renderiza.
     */
    bool m_isActive;

    /** * @brief Identificador único de la entidad. 
     */
    int m_id;

    /** * @brief Lista de componentes adjuntos.
     * Gestión automática de memoria mediante punteros compartidos.
     */
    std::vector<EU::TSharedPointer<Component>> m_components;
};