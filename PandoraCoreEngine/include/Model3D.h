#pragma once
#include "Prerequisites.h"
#include "IResource.h"
#include "MeshComponent.h"
#include "fbxsdk.h"

/**
 * @enum ModelType
 * @brief Defines the supported 3D model file formats.
 */
enum
ModelType {
  OBJ,    ///< Wavefront OBJ format.
  FBX     ///< Autodesk FBX format.
};

/**
 * @class Model3D
 * @brief Represents a 3D model resource loaded from a file (FBX/OBJ).
 *
 * This class handles the loading, parsing, and storage of 3D geometry data.
 * It integrates with the FBX SDK to import meshes, materials, and textures,
 * converting them into engine-compatible MeshComponents.
 */
class
Model3D : public IResource {
public:
  /**
   * @brief Constructs a Model3D resource.
   *
   * Automatically triggers the load process upon construction.
   *
   * @param name      Unique identifier for the resource.
   * @param modelType The format type of the model file.
   */
  Model3D(const std::string& name, ModelType modelType)
    : IResource(name),
      m_modelType(modelType),
      lSdkManager(nullptr),
      lScene(nullptr) {
    SetType(ResourceType::Model3D);
    load(name);
  }

  /**
   * @brief Default destructor.
   */
  ~Model3D() = default;

  /**
   * @brief Loads the model from the specified file path.
   *
   * @param path File system path to the 3D model.
   * @return True if loading and parsing were successful.
   */
  bool
  load(const std::string& path) override;
  
  /**
   * @brief Initializes GPU resources for the loaded model.
   * @return True if initialization succeeds.
   */
  bool
  init() override;
  
  /**
   * @brief Unloads the model data and releases memory.
   */
  void
  unload() override;
  
  /**
   * @brief Retrieves the size of the model in bytes.
   * @return Memory footprint size.
   */
  size_t
  getSizeInBytes() const override;

  /**
   * @brief Gets the list of mesh components generated from the model.
   * @return Constant reference to the vector of MeshComponents.
   */
  const std::vector<MeshComponent>&
  GetMeshes() const { return m_meshes; }

  /* FBX MODEL LOADER METHODS */

  /**
   * @brief Initializes the Autodesk FBX SDK Manager.
   * @return True if the manager was successfully created.
   */
  bool
  InitializeFBXManager();

  /**
   * @brief Loads and parses an FBX file.
   *
   * @param filePath Path to the .fbx file.
   * @return A vector of extracted MeshComponents.
   */
  std::vector<MeshComponent>
  LoadFBXModel(const std::string & filePath);

  /**
   * @brief Recursively processes a node in the FBX scene graph.
   * @param node Pointer to the current FBX node.
   */
  void
  ProcessFBXNode(FbxNode* node);

  /**
   * @brief Extracts mesh data from a specific FBX node.
   * @param node Pointer to the FBX node containing mesh geometry.
   */
  void
  ProcessFBXMesh(FbxNode* node);

  /**
   * @brief Processes material information associated with the geometry.
   * @param material Pointer to the FBX surface material.
   */
  void
  ProcessFBXMaterials(FbxSurfaceMaterial* material);

  /**
   * @brief Retrieves the file names of textures used by this model.
   * @return Vector of texture file name strings.
   */
  std::vector<std::string>
  GetTextureFileNames() const { return textureFileNames; }

private:
  FbxManager* lSdkManager;  ///< Pointer to the FBX SDK Manager instance.
  FbxScene* lScene;         ///< Pointer to the root FBX Scene object.
  std::vector<std::string> textureFileNames; ///< Cached list of textures.

public:
  ModelType m_modelType;                ///< Format type of the model.
  std::vector<MeshComponent> m_meshes;  ///< Geometry components.
};