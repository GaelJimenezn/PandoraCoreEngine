#pragma once
#include "Prerequisites.h"
#include "Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "SamplerState.h"
#include "ShaderProgram.h"

class Device;
class DeviceContext;
class MeshComponent;

/**
 * @class Actor
 * @brief Entidad especializada que representa un objeto renderizable.
 *
 * Gestiona mallas, texturas, shaders y buffers (Vértices, Índices) para
 * dibujar objetos 3D mediante la API gráfica.
 */
class Actor : public Entity {
public:
    Actor() = default;

    /**
     * @brief Constructor con dispositivo.
     * @param device Referencia para crear recursos gráficos.
     */
    Actor(Device& device);

    virtual ~Actor() = default;

    void 
    awake() override {}

    void 
    init() override {}

    /**
     * @brief Actualiza la lógica y prepara la World Matrix.
     */
    void 
    update(float deltaTime, DeviceContext& deviceContext) override;

    /**
     * @brief Renderiza el actor (binds y draw call).
     */
    void 
    render(DeviceContext& deviceContext) override;

    /**
     * @brief Libera buffers, texturas y shaders.
     */
    void 
    destroy() override;

    /**
     * @brief Asigna geometría y genera buffers en GPU.
     * @param device Dispositivo para crear buffers.
     * @param meshes Lista de datos de malla.
     */
    void 
    setMesh(Device& device, std::vector<MeshComponent> meshes);

    std::string 
    getName() { return m_name; }

    void 
    setName(const std::string& name) { m_name = name; }

    /**
     * @brief Asigna texturas (Materiales).
     */
    void 
    setTextures(std::vector<Texture> textures) { m_textures = textures; }

    /**
     * @brief Habilita/Deshabilita proyección de sombras.
     */
    void 
    setCastShadow(bool v) { castShadow = v; }

    bool 
    canCastShadow() const { return castShadow; }

    /**
     * @brief Renderizado exclusivo para el mapa de sombras.
     */
    void 
    renderShadow(DeviceContext& deviceContext);

private:
    // --- Recursos de Geometría ---
    std::vector<MeshComponent> m_meshes;
    std::vector<Texture> m_textures;
    std::vector<Buffer> m_vertexBuffers;
    std::vector<Buffer> m_indexBuffers;

    // --- Estados ---
    SamplerState m_sampler;
    
    // --- Shaders y Constantes ---
    CBChangesEveryFrame m_model;    ///< Datos para el Constant Buffer.
    Buffer m_modelBuffer;           ///< Recurso GPU del CB.

    // --- Sombras ---
    ShaderProgram m_shaderShadow;   ///< Shader de profundidad.
    Buffer m_shaderBuffer;          ///< CB para matriz luz.
    CBChangesEveryFrame m_cbShadow;
    XMFLOAT4 m_LightPos;
    
    // --- Metadatos ---
    std::string m_name = "Actor";
    bool castShadow = true;
};