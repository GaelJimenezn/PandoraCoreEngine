#pragma once
#include "Prerequisites.h"
class MeshComponent; 

class
ModelLoader {
public:
  /**
   * @brief Carga un modelo .obj desde un archivo.
   * @param filename Ruta al archivo .obj.
   * @param outMesh El componente Mesh donde se guardarán los datos.
   * @return true si la carga fue exitosa, false en caso contrario.
   */
  static bool
  loadFromFile(const std::string& filename, 
               MeshComponent& outMesh); // <-- PARÁMETRO 'outTextureName' ELIMINADO

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