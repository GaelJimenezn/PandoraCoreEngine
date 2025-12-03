#pragma once
#include "Prerequisites.h"
#include "IResource.h"

/**
 * @class ResourceManager
 * @brief Singleton manager for loading and caching engine resources.
 *
 * Centralizes the lifecycle management of resources (models, textures, etc.)
 * to ensure assets are loaded only once (Flyweight pattern) and properly
 * released when no longer needed.
 */
class
ResourceManager {
public:
  /**
   * @brief Default constructor.
   */
  ResourceManager()  = default;

  /**
   * @brief Default destructor.
   */
  ~ResourceManager() = default;

  /**
   * @brief Access the singleton instance.
   * @return Reference to the global ResourceManager instance.
   */
  static ResourceManager&
  getInstance() {
    static ResourceManager instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  /**
   * @brief Retrieves a resource from cache or loads it if missing.
   *
   * @tparam T    Resource type (must derive from IResource).
   * @tparam Args Variadic arguments passed to the resource constructor.
   *
   * @param key      Unique string identifier for the resource.
   * @param filename Path to the asset file.
   * @param args     Additional constructor arguments.
   * @return Shared pointer to the requested resource, or nullptr on failure.
   */
  template<typename T, typename... Args>
  std::shared_ptr<T>
  GetOrLoad(const std::string& key,
            const std::string& filename,
            Args&&... args) {
    static_assert(std::is_base_of<IResource, T>::value,
                  "T must derive from IResource");

    // 1. Check if resource exists in cache
    auto it = m_resources.find(key);
    if (it != m_resources.end()) {
      // Attempt cast to correct type
      auto existing = std::dynamic_pointer_cast<T>(it->second);
      if (existing && existing->GetState() == ResourceState::Loaded) {
        return existing; // Return cached instance
      }
    }

    // 2. Not found or invalid -> Create and load
    std::shared_ptr<T> resource =
      std::make_shared<T>(key, std::forward<Args>(args)...);

    if (!resource->load(filename)) {
      // Error handling can be expanded here
      return nullptr;
    }

    if (!resource->init()) {
      return nullptr;
    }

    // 3. Store in cache and return
    m_resources[key] = resource;
    return resource;
  }

  /**
   * @brief Retrieves a previously loaded resource.
   *
   * @tparam T Resource type.
   * @param key Unique string identifier.
   * @return Shared pointer to the resource if found, otherwise nullptr.
   */
  template<typename T>
  std::shared_ptr<T>
  Get(const std::string& key) const
  {
    auto it = m_resources.find(key);
    if (it == m_resources.end()) return nullptr;

    return std::dynamic_pointer_cast<T>(it->second);
  }

  /**
   * @brief Unloads and removes a specific resource from the cache.
   * @param key Identifier of the resource to unload.
   */
  void
  Unload(const std::string& key)
  {
    auto it = m_resources.find(key);
    if (it != m_resources.end()) {
      it->second->unload();
      m_resources.erase(it);
    }
  }

  /**
   * @brief Unloads all managed resources and clears the cache.
   */
  void
  UnloadAll()
  {
    for (auto& [key, res] : m_resources) {
      if (res) {
        res->unload();
      }
    }
    m_resources.clear();
  }

private:
  /// Map storing resources by their string key.
  std::unordered_map<std::string, std::shared_ptr<IResource>> m_resources;
};