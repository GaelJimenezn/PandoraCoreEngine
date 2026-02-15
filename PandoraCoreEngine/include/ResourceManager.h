#pragma once
#include "Prerequisites.h"
#include "IResource.h"

/**
 * @class ResourceManager
 * @brief Singleton que gestiona el ciclo de vida de los recursos del motor.
 * * Implementa el patrón Flyweight para evitar duplicados en memoria, 
 * centralizando la carga, el acceso y la liberación de recursos.
 */
class 
ResourceManager {
public:
	/** @brief Constructor por defecto. */
	ResourceManager()  = default;

	/** @brief Destructor por defecto. */
	~ResourceManager() = default;

	/**
	 * @brief Obtiene la instancia única del gestor (Singleton).
	 * @return Referencia a la instancia de ResourceManager.
	 */
	static ResourceManager& 
		getInstance() {
		static ResourceManager instance;
		return instance;
	}

	/// Bloqueo de constructor de copia.
	ResourceManager(const ResourceManager&) = delete;

	/// Bloqueo de operador de asignación.
	ResourceManager& 
		operator=(const ResourceManager&) = delete;

	/**
	 * @brief Obtiene o carga un recurso de tipo T.
	 * * Si el recurso ya existe en el caché, lo devuelve. Si no, lo crea,
	 * lo carga desde el archivo y lo inicializa.
	 * * @tparam T Tipo de recurso (debe heredar de IResource).
	 * @param key Identificador único para el caché.
	 * @param filename Ruta del archivo a cargar.
	 * @param args Argumentos adicionales para el constructor de T.
	 * @return shared_ptr al recurso o nullptr si falla la carga.
	 */
	template<typename T, typename... Args>
	std::shared_ptr<T> 
		GetOrLoad(const std::string& key,
				  const std::string& filename,
				  Args&&... args) {
		static_assert(std::is_base_of<IResource, T>::value,
					  "T debe heredar de IResource");

		// 1. ¿Ya existe el recurso en el caché?
		auto it = m_resources.find(key);
		if (it != m_resources.end()) {
			auto existing = std::dynamic_pointer_cast<T>(it->second);
			if (existing && existing->GetState() == ResourceState::Loaded) {
				return existing; // Flyweight: reutilizamos
			}
		}

		// 2. No existe -> crearlo y cargarlo
		std::shared_ptr<T> resource = 
			std::make_shared<T>(key, std::forward<Args>(args)...);

		if (!resource->load(filename)) {
			return nullptr;
		}

		if (!resource->init()) {
			return nullptr;
		}

		// 3. Guardar en el caché
		m_resources[key] = resource;
		return resource;
	}

	/**
	 * @brief Busca un recurso ya cargado sin intentar cargarlo.
	 * @tparam T Tipo del recurso esperado.
	 * @param key Clave del recurso.
	 * @return shared_ptr al recurso o nullptr si no se encuentra.
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
	 * @brief Descarga y elimina un recurso específico del caché.
	 * @param key Clave del recurso a liberar.
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
	 * @brief Libera todos los recursos almacenados en el gestor.
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
	/** @brief Mapa de recursos indexados por una clave única. */
	std::unordered_map<std::string, 
					   std::shared_ptr<IResource>> m_resources;
};