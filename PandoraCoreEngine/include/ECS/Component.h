#pragma once
#include "Prerequisites.h"

class DeviceContext;

/**
 * @class Component
 * @brief Clase base abstracta para componentes funcionales del juego.
 *
 * Define la interfaz que cualquier comportamiento o dato adjunto a una
 * Entidad debe implementar.
 */
class Component {
public:
    /**
     * @brief Constructor por defecto (Tipo NONE).
     */
    Component() = default;

    /**
     * @brief Constructor con asignación de tipo.
     * @param type Identificador del tipo de componente.
     */
    Component(const ComponentType type) : m_type(type) {}

    /**
     * @brief Destructor virtual.
     */
    virtual ~Component() = default;

    /**
     * @brief Inicializa el componente.
     *
     * Se ejecuta tras adjuntar el componente a una entidad.
     */
    virtual void 
    init() = 0;

    /**
     * @brief Actualiza la lógica del componente.
     * @param deltaTime Tiempo (segundos) desde el último frame.
     */
    virtual void 
    update(float deltaTime) = 0;

    /**
     * @brief Renderiza elementos visuales.
     * @param deviceContext Contexto gráfico para dibujar.
     */
    virtual void 
    render(DeviceContext& deviceContext) = 0;

    /**
     * @brief Destruye el componente y libera recursos manuales.
     */
    virtual void 
    destroy() = 0;

    /**
     * @brief Obtiene el identificador de tipo.
     * @return ComponentType Enum correspondiente al tipo.
     */
    ComponentType 
    getType() const { return m_type; }

protected:
    /** * @brief Tipo de componente para identificación en runtime. 
     */
    ComponentType m_type = ComponentType::NONE; 
};