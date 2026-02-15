#pragma once
#include "Prerequisites.h"

class Entity;
class DeviceContext;

/**
 * @class SceneGraph
 * @brief Gestor de la jerarquía de entidades en la escena.
 *
 * Administra el árbol de nodos de la escena, gestionando las relaciones
 * padre-hijo y propagando las transformaciones (World Matrices) de padres
 * a hijos.
 */
class SceneGraph {
public:
    SceneGraph() = default;
    ~SceneGraph() = default;

    /**
     * @brief Inicializa el grafo de escena.
     */
    void 
    init();

    /**
     * @brief Registra una entidad en el grafo.
     * @param e Puntero a la entidad a agregar.
     */
    void 
    addEntity(Entity* e);

    /**
     * @brief Elimina una entidad del grafo.
     * @param e Puntero a la entidad a remover.
     */
    void 
    removeEntity(Entity* e);

    /**
     * @brief Verifica si una entidad es ancestro de otra.
     * Útil para evitar ciclos en la jerarquía.
     * @param possibleAncestor Entidad candidata a ancestro.
     * @param node Entidad descendiente.
     */
    bool 
    isAncestor(Entity* possibleAncestor, Entity* node) const;

    /**
     * @brief Emparenta una entidad hija a un padre.
     * @param child Entidad que será hija.
     * @param parent Entidad que será padre.
     * @return true si la operación fue exitosa (sin ciclos).
     */
    bool
    attach(Entity* child, Entity* parent);

    /**
     * @brief Desvincula una entidad de su padre (la hace huérfana/raíz).
     * @param child Entidad a desvincular.
     */
    bool
    detach(Entity* child);

    /**
     * @brief Actualiza la lógica y las transformaciones de todas las entidades.
     * Realiza el recorrido del árbol para actualizar matrices mundiales.
     * @param deltaTime Tiempo transcurrido.
     * @param deviceContext Contexto del dispositivo.
     */
    void 
    update(float deltaTime, DeviceContext& deviceContext);
    
    /**
     * @brief Renderiza todas las entidades del grafo.
     * @param deviceContext Contexto para operaciones de dibujo.
     */
    void 
    render(DeviceContext& deviceContext);

    /**
     * @brief Limpia el grafo y libera referencias.
     */
    void
    destroy();

private:
    /**
     * @brief Método recursivo para actualizar la matriz de mundo.
     * Propaga la transformación del padre a los hijos.
     * @param node Entidad actual.
     * @param parentWorld Matriz de mundo acumulada del padre.
     */
    void 
    updateWorldRecursive(Entity* node, const XMMATRIX& parentWorld);

    /**
     * @brief Verifica si una entidad es raíz (sin padre).
     */
    bool 
    isRoot(Entity* e) const;

    /**
     * @brief Verifica si la entidad ya está registrada en el sistema.
     */
    bool
    isRegistered(Entity* e) const;

private:
    // std::vector<EU::TSharedPointer<Entity>> m_entities; // (Comentado)

public:
    /** @brief Lista plana de entidades registradas para acceso rápido. */
    std::vector<Entity*> m_entities;
};