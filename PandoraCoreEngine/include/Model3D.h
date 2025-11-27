#pragma once
#include "Prerequisites.h"
#include "IResource.h"
#include "MeshComponent.h"
#include "fbxsdk.h"

/**
 * @enum ModelType
 * @brief Define el tipo de modelo a cargar.
 * Aunque el SDK de FBX autodetecta el formato, esto ayuda a la organización.
 */
enum 
ModelType {
  OBJ = 0,
  FBX = 1
};

/**
 * @class Model3D
 * @brief Clase recurso que gestiona la carga y almacenamiento de modelos 3D.
 * @details Esta clase unifica la lógica de recurso (IResource) con la lógica de carga
 * (anteriormente ModelLoader), utilizando el SDK de FBX para procesar archivos
 * .fbx y .obj, extrayendo geometría, UVs y normales.
 */
class 
Model3D : public IResource {
public:
  /**
   * @brief Constructor de Model3D.
   * @details Inicializa el recurso e intenta cargarlo inmediatamente.
   * @param name Nombre o ruta del archivo del modelo.
   * @param modelType Tipo de modelo (FBX por defecto).
   */
  Model3D(const std::string& name, ModelType modelType = ModelType::FBX) 
  : IResource(name), m_modelType(modelType), lSdkManager(nullptr), lScene(nullptr) {
    SetType(ResourceType::Model3D);
    load(name);
  }

  /**
   * @brief Destructor por defecto.
   */
  ~Model3D() = default;

  /**
   * @brief Implementación de la carga del recurso.
   * @param path Ruta al archivo del modelo.
   * @return true si la carga fue exitosa, false en caso contrario.
   */
  bool 
  load(const std::string& path) override;
  
  /**
   * @brief Inicializa los datos del modelo tras la carga.
   * @details Aquí se invoca la lógica principal del SDK de FBX.
   * @return true si la inicialización fue exitosa.
   */
  bool 
  init() override;
  
  /**
   * @brief Libera la memoria y los recursos del SDK.
   */
  void 
  unload() override;
  
  /**
   * @brief Calcula el tamaño aproximado del recurso en bytes.
   * @return Tamaño en bytes.
   */
  size_t 
  getSizeInBytes() const override;

  /**
   * @brief Obtiene las mallas generadas por el cargador.
   * @return Referencia constante al vector de MeshComponent.
   */
  const std::vector<MeshComponent>& 
  GetMeshes() const { return m_meshes; }

  /**
   * @brief Obtiene los nombres de las texturas encontradas en el modelo.
   * @return Vector de strings con los nombres de archivo.
   */
  std::vector<std::string> 
  GetTextureFileNames() const { return textureFileNames; }

private:
  /* --- FUNCIONES INTERNAS DE CARGA FBX (Lógica del Loader) --- */

  /**
   * @brief Inicializa el FbxManager y la FbxScene.
   * @return true si se crearon correctamente el manager y la escena.
   */
  bool
  InitializeFBXManager();

  /**
   * @brief Carga el archivo utilizando el FbxImporter.
   * @details Configura el importador, carga la escena, convierte el sistema de ejes
   * y triangula la geometría.
   * @param filePath Ruta del archivo a importar.
   * @return Vector de mallas generadas (aunque se almacenan internamente en m_meshes).
   */
  std::vector<MeshComponent>
  LoadFBXModel(const std::string & filePath);

  /**
   * @brief Recorre recursivamente los nodos de la escena FBX.
   * @param node Puntero al nodo actual a procesar.
   */
  void 
  ProcessFBXNode(FbxNode* node);

  /**
   * @brief Procesa un nodo de tipo Malla (Mesh).
   * @details Extrae vértices, índices, normales y coordenadas de textura,
   * convirtiéndolos al formato SimpleVertex del motor.
   * @param node Puntero al nodo que contiene la malla.
   */
  void 
  ProcessFBXMesh(FbxNode* node);

  /**
   * @brief Procesa los materiales asociados a un nodo.
   * @details Busca propiedades de textura difusa y almacena sus nombres.
   * @param material Puntero al material de superficie FBX.
   */
  void 
  ProcessFBXMaterials(FbxSurfaceMaterial* material);

private:
  FbxManager* lSdkManager;
  FbxScene* lScene;
  std::vector<std::string> textureFileNames;

public:
  ModelType m_modelType;
  std::vector<MeshComponent> m_meshes;
};