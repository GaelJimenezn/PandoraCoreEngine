#pragma once
#include "Prerequisites.h"

/**
 * @enum ResourceType
 * @brief Categorías de recursos gestionados por el motor.
 */
enum class ResourceType {
    Unknown,    ///< Tipo desconocido o no inicializado.
    Model3D,    ///< Modelo tridimensional (Malla).
    Texture,    ///< Imagen o textura.
    Sound,      ///< Clip de audio.
    Shader,     ///< Programa de sombreado.
    Material    ///< Definición de superficie.
};

/**
 * @enum ResourceState
 * @brief Estado actual del ciclo de vida del recurso.
 */
enum class ResourceState {
    Unloaded,   ///< El recurso no está en memoria.
    Loading,    ///< El recurso se está cargando (posiblemente asíncrono).
    Loaded,     ///< El recurso está listo para usarse.
    Failed      ///< Hubo un error al cargar el recurso.
};

/**
 * @class IResource
 * @brief Interfaz base para cualquier recurso cargable del motor.
 *
 * Define la estructura común para identificar, cargar y liberar recursos
 * como texturas, modelos o sonidos.
 */
class IResource {
public:
    /**
     * @brief Constructor base.
     * @param name Nombre único o identificador del recurso.
     */
    IResource(const std::string& name)
        : m_name(name)
        , m_filePath("")
        , m_type(ResourceType::Unknown)
        , m_state(ResourceState::Unloaded)
        , m_id(GenerateID())
    {
    }

    virtual ~IResource() = default;

    /**
     * @brief Inicializa el recurso en la API gráfica (GPU).
     * @return true si la inicialización fue exitosa.
     */
    virtual bool 
    init() = 0;

    /**
     * @brief Carga los datos del recurso desde el disco (CPU).
     * @param filename Ruta del archivo a cargar.
     * @return true si la carga fue exitosa.
     */
    virtual bool 
    load(const std::string& filename) = 0;

    /**
     * @brief Libera la memoria (CPU y GPU) asociada al recurso.
     */
    virtual void 
    unload() = 0;

    /**
     * @brief Obtiene el tamaño del recurso en bytes.
     * Útil para estadísticas de memoria y profiler.
     */
    virtual size_t 
    getSizeInBytes() const = 0;

    // --- Setters ---
    void SetPath(const std::string& path) { m_filePath = path; }
    void SetType(ResourceType t) { m_type = t; }
    void SetState(ResourceState s) { m_state = s; }

    // --- Getters ---
    const std::string& GetName() const { return m_name; }
    const std::string& GetPath() const { return m_filePath; }
    ResourceType GetType() const { return m_type; }
    ResourceState GetState() const { return m_state; }
    uint64_t GetID() const { return m_id; }

protected:
    std::string m_name;     ///< Nombre lógico del recurso.
    std::string m_filePath; ///< Ruta física en disco.
    ResourceType m_type;    ///< Tipo de recurso.
    ResourceState m_state;  ///< Estado de carga.
    uint64_t m_id;          ///< ID único autogenerado.

private:
    /**
     * @brief Generador interno de IDs únicos.
     */
    static uint64_t GenerateID()
    {
        static uint64_t nextID = 1;
        return nextID++;
    }
};