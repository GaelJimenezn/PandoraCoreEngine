#include "Model3D.h"

bool 
Model3D::load(const std::string& path) {
  SetPath(path);
  SetState(ResourceState::Loading);

  // Llamamos a init que contiene la lógica de carga real
  bool success = init();

  SetState(success ? ResourceState::Loaded : ResourceState::Failed);
  return success;
}

bool 
Model3D::init() {
  // Disparamos la carga del modelo con el SDK
  auto meshes = LoadFBXModel(m_filePath);
  
  if (!meshes.empty()) {
    return true;
  }
  return false;
}

void 
Model3D::unload() {
  m_meshes.clear();
  textureFileNames.clear();
  
  // Limpieza segura del manager de FBX
  if (lSdkManager) {
    lSdkManager->Destroy();
    lSdkManager = nullptr;
  }
  
  SetState(ResourceState::Unloaded);
}

size_t 
Model3D::getSizeInBytes() const {
  size_t size = sizeof(*this);
  for(const auto& mesh : m_meshes) {
    size += mesh.m_vertex.size() * sizeof(SimpleVertex);
    size += mesh.m_index.size() * sizeof(unsigned int);
  }
  return size;
}

// -------------------------------------------------------------------------
// IMPLEMENTACIÓN DE LA LÓGICA DE CARGA (FBX SDK)
// -------------------------------------------------------------------------

bool
Model3D::InitializeFBXManager() {
  lSdkManager = FbxManager::Create();
  if (!lSdkManager) {
    ERROR(L"Model3D", L"InitializeFBXManager", L"Unable to create FBX Manager!");
    return false;
  }

  FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
  lSdkManager->SetIOSettings(ios);

  lScene = FbxScene::Create(lSdkManager, "MyScene");
  if (!lScene) {
    ERROR(L"Model3D", L"InitializeFBXManager", L"Unable to create FBX Scene!");
    return false;
  }
  return true;
}

std::vector<MeshComponent> 
Model3D::LoadFBXModel(const std::string& filePath) {
  m_meshes.clear();

  if (InitializeFBXManager()) {
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
    if (!lImporter) return std::vector<MeshComponent>();

    // Inicializar el importador
    if (!lImporter->Initialize(filePath.c_str(), -1, lSdkManager->GetIOSettings())) {
      std::string err = lImporter->GetStatus().GetErrorString();
      std::wstring wErr(err.begin(), err.end());
      ERROR(L"Model3D", L"LoadFBXModel", wErr.c_str());
      lImporter->Destroy();
      return std::vector<MeshComponent>();
    }

    // Importar la escena
    if (!lImporter->Import(lScene)) {
      ERROR(L"Model3D", L"LoadFBXModel", L"Unable to import FBX Scene!");
      lImporter->Destroy();
      return std::vector<MeshComponent>();
    }

    // Convertir ejes y unidades para DirectX
    FbxAxisSystem::DirectX.ConvertScene(lScene);
    FbxSystemUnit::m.ConvertScene(lScene); 

    // Triangular geometría (paso crítico)
    FbxGeometryConverter gc(lSdkManager);
    gc.Triangulate(lScene, /*replace*/ true);

    lImporter->Destroy();

    // Procesar nodos
    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode) {
      for (int i = 0; i < lRootNode->GetChildCount(); i++) {
        ProcessFBXNode(lRootNode->GetChild(i));
      }
    }
  }
  return m_meshes;
}

void 
Model3D::ProcessFBXNode(FbxNode* node) {
  if (node->GetNodeAttribute()) {
    if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
      ProcessFBXMesh(node);
    }
    
    // Procesar materiales si existen
    int materialCount = node->GetMaterialCount();
    for (int i = 0; i < materialCount; ++i) {
      ProcessFBXMaterials(node->GetMaterial(i));
    }
  }

  for (int i = 0; i < node->GetChildCount(); i++) {
    ProcessFBXNode(node->GetChild(i));
  }
}

void 
Model3D::ProcessFBXMesh(FbxNode* node) {
  FbxMesh* mesh = node->GetMesh();
  if (!mesh) return;

  // Generar normales si faltan
  if (mesh->GetElementNormalCount() == 0)
    mesh->GenerateNormals(true, true);

  const char* uvSetName = nullptr;
  FbxStringList uvSets;
  mesh->GetUVSetNames(uvSets);
  if (uvSets.GetCount() > 0) uvSetName = uvSets[0];

  // Buffers temporales
  std::vector<SimpleVertex> vertices;
  std::vector<unsigned int> indices;

  int polygonCount = mesh->GetPolygonCount();
  int vertexCounter = 0;

  for (int p = 0; p < polygonCount; ++p) {
    int polySize = mesh->GetPolygonSize(p); // Debería ser 3 tras triangular

    for (int v = 0; v < polySize; ++v) {
      int cpIndex = mesh->GetPolygonVertex(p, v);
      
      SimpleVertex vertex{};

      // 1. Posición
      FbxVector4 pos = mesh->GetControlPointAt(cpIndex);
      vertex.Pos = { (float)pos[0], (float)pos[1], (float)pos[2] };

      // 2. UVs (Invertir V para DirectX)
      if (uvSetName) {
        FbxVector2 uv;
        bool unmapped;
        mesh->GetPolygonVertexUV(p, v, uvSetName, uv, unmapped);
        vertex.Tex = { (float)uv[0], 1.0f - (float)uv[1] };
      } else {
        vertex.Tex = { 0.0f, 0.0f };
      }

      // 3. Normales
      FbxVector4 norm;
      mesh->GetPolygonVertexNormal(p, v, norm);
      vertex.Normal = { (float)norm[0], (float)norm[1], (float)norm[2] };

      vertices.push_back(vertex);
      indices.push_back(vertexCounter);
      vertexCounter++;
    }
  }

  MeshComponent mc;
  mc.m_name = node->GetName();
  mc.m_vertex = std::move(vertices);
  mc.m_index = std::move(indices);
  mc.m_numVertex = (int)mc.m_vertex.size();
  mc.m_numIndex = (int)mc.m_index.size();

  m_meshes.push_back(std::move(mc));
}

void 
Model3D::ProcessFBXMaterials(FbxSurfaceMaterial* material) {
  if (material) {
    FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (prop.IsValid()) {
      int textureCount = prop.GetSrcObjectCount<FbxTexture>();
      for (int i = 0; i < textureCount; ++i) {
        FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(i));
        if (texture) {
          textureFileNames.push_back(texture->GetName());
        }
      }
    }
  }
}