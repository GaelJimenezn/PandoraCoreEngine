#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"

class DeviceContext;
class Entity;

/**
 * @class HierarchyComponent
 * @brief Componente que dota a una entidad de capacidades jerárquicas.
 *
 * Almacena referencias al padre y a los hijos, permitiendo construir
 * el árbol de escena.
 */
class HierarchyComponent : public Component {
public:
    /**
     * @brief Constructor. Asigna el tipo HIERARCHY.
     */
    HierarchyComponent() : Component(ComponentType::HIERARCHY) {}
    
    ~HierarchyComponent() = default;

    void 
    init() override {}
    
    void 
    update(float) override {}
    
    void 
    render(DeviceContext& deviceContext) override {}

    /**
     * @brief Destruye el componente y limpia relaciones.
     * Rompe los enlaces con padre e hijos para evitar punteros colgantes.
     */
    void 
    destroy() override { 
        m_children.clear(); 
        m_parent = nullptr; 
    }

    // --- API SceneGraph ---

    /**
     * @brief Asigna un nuevo padre.
     * @param parent Puntero a la entidad padre.
     */
    void 
    setParent(Entity* parent) { 
        m_parent = parent; 
    }

    /**
     * @brief Indica si la entidad no tiene padre.
     */
    bool 
    isRoot() const {
        return m_parent == nullptr;
    }
    
    /**
     * @brief Indica si la entidad tiene hijos adjuntos.
     */
    bool 
    hasChildren() const {
        return !m_children.empty();
    }

    /**
     * @brief Agrega un hijo a la lista.
     * Evita duplicados.
     * @param child Puntero a la entidad hija.
     */
    void 
    addChild(Entity* child) {
        if(!child) {
            return;
        }

        if (std::find(m_children.begin(), 
                      m_children.end(), 
                      child) != m_children.end()) {
            return;
        }
        m_children.push_back(child);
    }

    /**
     * @brief Elimina un hijo de la lista.
     * @param child Puntero a la entidad hija a remover.
     */
    void
    removeChild(Entity* child) {
        if (!child) return;

        m_children.erase(
            std::remove(m_children.begin(), m_children.end(), child),
            m_children.end()
        );
    }

public:
    Entity* m_parent = nullptr;      ///< Puntero a la entidad padre (o null).
    std::vector<Entity*> m_children; ///< Lista de punteros a entidades hijas.
};