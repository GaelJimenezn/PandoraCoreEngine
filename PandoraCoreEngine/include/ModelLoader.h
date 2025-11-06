#pragma once
#include "Prerequisites.h"
class MeshComponent; 

/**
 * @class ModelLoader
 * @brief Clase de utilidad estática para cargar modelos 3D desde archivos .obj.
 * @details Proporciona métodos para parsear la geometría (vértices, texturas,
 * normales) y las caras de un archivo .obj, optimizando los datos para
 * su renderizado.
 */
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
               MeshComponent& outMesh); 

private:
  /**
   * @brief Procesa una única cara (triángulo o quad) leída del archivo .obj.
   * @details Descompone los índices de la cara (v/vt/vn), crea nuevos
   * vértices si es necesario (o reutiliza existentes) y genera los índices
   * para la triangulación.
   * @param faceData Vector de strings, cada uno representando un vértice
   * (ej: "1/1/1", "2/2/2").
   * @param uniqueVertices Mapa para rastrear combinaciones únicas de v/vt/vn
   * y optimizar vértices.
   * @param outVertices El vector de vértices final al que se añaden nuevos
   * SimpleVertex.
   * @param outIndices El vector de índices final al que se añaden los
   * índices de la cara triangulada.
   * @param temp_positions Vector de todas las posiciones 'v' leídas del
   * archivo.
   * @param temp_texcoords Vector de todas las coordenadas 'vt' leídas del
   * archivo.
   * @param temp_normals Vector de todas las normales 'vn' leídas del
   * archivo.
   */
  static void
  processFace(const std::vector<std::string>& faceData,
              std::unordered_map<std::string, unsigned int>& uniqueVertices,
              std::vector<SimpleVertex>& outVertices,
              std::vector<unsigned int>& outIndices,
              const std::vector<XMFLOAT3>& temp_positions,
              const std::vector<XMFLOAT2>& temp_texcoords,
              const std::vector<XMFLOAT3>& temp_normals);
};