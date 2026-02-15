#pragma once
#include "Prerequisites.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "Component.h"

class DeviceContext;

/**
 * @class Transform
 * @brief Define posición, rotación y escala en el espacio 3D.
 *
 * Calcula la World Matrix. Es fundamental para objetos renderizables.
 */
class Transform : public Component {
public:
    /**
     * @brief Constructor por defecto.
     */
    Transform() : position(),
        rotation(),
        scale(),
        matrix(),
        Component(ComponentType::TRANSFORM) {
    }

    virtual ~Transform() = default;

    /**
     * @brief Inicializa valores por defecto (Escala 1, Matriz Identidad).
     */
    void 
    init() override {
        scale.one();
        matrix = XMMatrixIdentity();
    }

    /**
     * @brief Calcula la matriz de transformación final.
     *
     * Orden: Escala -> Rotación (X*Y*Z) -> Traslación.
     * @param deltaTime Tiempo transcurrido.
     */
    void 
    update(float deltaTime) override {
        // 1. Escala
        XMMATRIX scaM = XMMatrixScaling(scale.x, scale.y, scale.z);

        // 2. Rotación (Por ejes para evitar Gimbal Lock visual en editores)
        XMMATRIX rotX = XMMatrixRotationX(rotation.x);
        XMMATRIX rotY = XMMatrixRotationY(rotation.y);
        XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);
        XMMATRIX rotM = rotX * rotY * rotZ;

        // 3. Traslación
        XMMATRIX traM = XMMatrixTranslation(position.x, 
                                            position.y, 
                                            position.z);

        // Componer: Scale -> Rotation -> Translation
        matrix = scaM * rotM * traM;
    }

    void 
    render(DeviceContext& deviceContext) override {}

    void 
    destroy() override {}

    // --- Getters y Setters ---

    const EU::Vector3& 
    getPosition() const { return position; }

    void 
    setPosition(const EU::Vector3& newPos) { position = newPos; }

    const EU::Vector3& 
    getRotation() const { return rotation; }

    void 
    setRotation(const EU::Vector3& newRot) { rotation = newRot; }

    const EU::Vector3& 
    getScale() const { return scale; }

    void 
    setScale(const EU::Vector3& newScale) { scale = newScale; }

    /**
     * @brief Configura todo el transform de una vez.
     */
    void 
    setTransform(const EU::Vector3& pos,
                 const EU::Vector3& rot,
                 const EU::Vector3& sca) {
        position = pos;
        rotation = rot;
        scale = sca;
    }

    /**
     * @brief Desplaza la entidad.
     * @param d Vector de desplazamiento.
     */
    void 
    translate(const EU::Vector3& d);

public:
    /** @brief Matriz de transformación final (World Matrix). */
    XMMATRIX matrix;

private:
    EU::Vector3 position; ///< Posición local.
    EU::Vector3 rotation; ///< Rotación Euler (Radianes).
    EU::Vector3 scale;    ///< Escala local.
};