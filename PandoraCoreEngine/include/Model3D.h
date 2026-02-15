#pragma once
#include "Prerequisites.h"
#include "IResource.h"
#include "MeshComponent.h"
#include "fbxsdk.h"

/**
 * @enum ModelType
 * @brief Formatos de modelos 3D soportados.
 */
enum
	ModelType {
	OBJ, /**< Formato Wavefront OBJ. */
	FBX  /**< Formato Autodesk FBX. */
};

/**
 * @class Model3D
 * @brief Gestiona la carga y recursos de modelos tridimensionales.
 *
 * Hereda de IResource para integrarse en el sistema de recursos
 * y utiliza el SDK de FBX para procesar mallas y materiales.
 */
class
Model3D : public IResource {
public:
	/**
	 * @brief Constructor de Model3D.
	 * @param name Nombre del recurso.
	 * @param modelType Tipo (OBJ/FBX).
	 */
	Model3D(const std::string& name, ModelType modelType)
		: IResource(name), m_modelType(modelType), 
		  lSdkManager(nullptr), lScene(nullptr) {
		SetType(ResourceType::Model3D);
		load(name);
	}

	/** @brief Destructor por defecto. */
	~Model3D() = default;

	/**
	 * @brief Carga el modelo desde disco.
	 * @param path Ruta del archivo.
	 * @return true si tuvo éxito.
	 */
	bool
	load(const std::string& path) override;

	/**
	 * @brief Inicializa buffers y recursos.
	 * @return Estado de la operación.
	 */
	bool
	init() override;

	/** @brief Libera la memoria del modelo. */
	void
	unload() override;

	/**
	 * @brief Tamaño del recurso en memoria.
	 * @return Bytes totales.
	 */
	size_t
	getSizeInBytes() const override;

	/**
	 * @brief Retorna las mallas cargadas.
	 * @return Lista de MeshComponent.
	 */
	const std::vector<MeshComponent>&
	GetMeshes() const { return m_meshes; }

	/* FBX MODEL LOADER*/

	/**
	 * @brief Inicia el SDK Manager de FBX.
	 * @return true si se inicializó.
	 */
	bool
	InitializeFBXManager();

	/**
	 * @brief Carga y parsea un archivo FBX.
	 * @param filePath Ruta al archivo.
	 * @return Vector con mallas.
	 */
	std::vector<MeshComponent>
	LoadFBXModel(const std::string& filePath);

	/**
	 * @brief Procesa nodos de la escena FBX.
	 * @param node Nodo a procesar.
	 */
	void
	ProcessFBXNode(FbxNode* node);

	/**
	 * @brief Extrae geometría de un nodo.
	 * @param node Nodo con malla.
	 */
	void
	ProcessFBXMesh(FbxNode* node);

	/**
	 * @brief Extrae datos de materiales.
	 * @param material Material FBX.
	 */
	void
	ProcessFBXMaterials(FbxSurfaceMaterial* material);

	/**
	 * @brief Obtiene nombres de texturas.
	 * @return Lista de strings.
	 */
	std::vector<std::string>
	GetTextureFileNames() const { return textureFileNames; }

private:
	FbxManager* lSdkManager; /**< Gestor del SDK. */
	FbxScene* lScene;       /**< Escena FBX. */
	std::vector<std::string> textureFileNames; /**< Texturas. */

public:
	ModelType m_modelType; /**< Tipo de modelo. */
	std::vector<MeshComponent> m_meshes; /**< Mallas. */
};