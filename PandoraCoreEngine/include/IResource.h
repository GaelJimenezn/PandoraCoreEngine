#pragma once
#include "Prerequisites.h"

/**
 * @enum ResourceType
 * @brief Identifies the category of a managed resource.
 */
enum class
ResourceType {
  Unknown,    ///< Type not specified.
  Model3D,    ///< 3D Geometry model.
  Texture,    ///< Image texture.
  Sound,      ///< Audio clip.
  Shader,     ///< Shader program.
  Material    ///< Material definition.
};

/**
 * @enum ResourceState
 * @brief Tracks the lifecycle state of a resource.
 */
enum class
ResourceState {
  Unloaded,   ///< Not yet loaded into memory.
  Loading,    ///< Currently in the loading process.
  Loaded,     ///< Successfully loaded and ready for use.
  Failed      ///< Loading failed.
};

/**
 * @class IResource
 * @brief Abstract base class defining the interface for all engine resources.
 *
 * Provides a common contract for initialization, loading from disk, and
 * memory management (unloading). It also tracks metadata like name, path,
 * type, and unique IDs.
 */
class
IResource {
public:
  /**
   * @brief Constructor.
   * @param name Unique identifier or friendly name for the resource.
   */
  IResource(const std::string& name)
    : m_name(name)
    , m_filePath("")
    , m_type(ResourceType::Unknown)
    , m_state(ResourceState::Unloaded)
    , m_id(GenerateID())
  {	}

  /**
   * @brief Virtual destructor.
   */
  virtual
  ~IResource() = default;

  /**
   * @brief Initializes GPU-side resources.
   * @return True if successful, false otherwise.
   */
  virtual bool
  init() = 0;

  /**
   * @brief Loads the resource data from the file system.
   * @param filename Relative or absolute path to the asset file.
   * @return True if the file was loaded successfully.
   */
  virtual bool
  load(const std::string& filename) = 0;

  /**
   * @brief Releases memory and resources associated with this asset.
   */
  virtual void
  unload() = 0;

  /**
   * @brief Retrieves the memory footprint of the resource.
   * @return Size in bytes. Used for profiling.
   */
  virtual size_t
  getSizeInBytes() const = 0;

  // Setters
  void
  SetPath(const std::string& path) { m_filePath = path; }
  
  void
  SetType(ResourceType t) { m_type = t; }
  
  void
  SetState(ResourceState s) { m_state = s; }

  // Getters
  const std::string&
  GetName() const { return m_name; }

  const std::string&
  GetPath() const { return m_filePath; }

  ResourceType
  GetType() const { return m_type; }

  ResourceState
  GetState() const { return m_state; }

  uint64_t
  GetID() const { return m_id; }

protected:
  std::string m_name;      ///< Resource name.
  std::string m_filePath;  ///< File system path.
  ResourceType m_type;     ///< Type category.
  ResourceState m_state;   ///< Current lifecycle state.
  uint64_t m_id;           ///< Unique ID.

private:
  /**
   * @brief Generates a unique thread-safe ID for resources.
   * @return A new unique 64-bit integer.
   */
  static uint64_t
  GenerateID()
  {
    static uint64_t nextID = 1;
    return nextID++;
  }
};