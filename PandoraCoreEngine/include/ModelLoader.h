#pragma once
#include "Prerequisites.h"
class MeshComponent; 

class
ModelLoader {
public:
  static bool
  loadFromFile(const std::string& filename, 
               MeshComponent& outMesh,
               std::string& outTextureName);

private:
  static void
  processFace(const std::vector<std::string>& faceData,
              std::unordered_map<std::string, unsigned int>& uniqueVertices,
              std::vector<SimpleVertex>& outVertices,
              std::vector<unsigned int>& outIndices,
              const std::vector<XMFLOAT3>& temp_positions,
              const std::vector<XMFLOAT2>& temp_texcoords,
              const std::vector<XMFLOAT3>& temp_normals);
};