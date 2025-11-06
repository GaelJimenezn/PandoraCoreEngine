#include "ModelLoader.h"
#include "MeshComponent.h" 

static std::vector<std::string>
split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void
ModelLoader::processFace(
    const std::vector<std::string>& faceData,
    std::unordered_map<std::string, unsigned int>& uniqueVertices,
    std::vector<SimpleVertex>& outVertices,
    std::vector<unsigned int>& outIndices,
    const std::vector<XMFLOAT3>& temp_positions,
    const std::vector<XMFLOAT2>& temp_texcoords,
    const std::vector<XMFLOAT3>& temp_normals) { 

  std::vector<unsigned int> faceIndices;

  for (const std::string& vertexString : faceData) {
    if (uniqueVertices.count(vertexString)) {
      faceIndices.push_back(uniqueVertices[vertexString]);
    }
    else {
      std::vector<std::string> indices = split(vertexString, '/');
      
      try {
        SimpleVertex newVert = {};
        int posIdx = -1, texIdx = -1, normIdx = -1;

        if (indices.empty() || indices[0].empty()) continue; 
        posIdx = std::stoi(indices[0]) - 1;
        
        // Validación
        if (posIdx < 0 || posIdx >= temp_positions.size()) continue;
        newVert.Pos = temp_positions[posIdx];

        if (indices.size() > 1 && !indices[1].empty()) {
          texIdx = std::stoi(indices[1]) - 1;
          if (texIdx >= 0 && texIdx < temp_texcoords.size()) {
            newVert.Tex = temp_texcoords[texIdx];
          } else {
            newVert.Tex = XMFLOAT2(0.0f, 0.0f); // Default
          }
        } else {
   
          newVert.Tex = XMFLOAT2(0.0f, 0.0f); // Default
        }

  
        if (indices.size() > 2 && !indices[2].empty()) {
          normIdx = std::stoi(indices[2]) - 1;
          if (normIdx >= 0 && normIdx < temp_normals.size()) {
            newVert.Normal = temp_normals[normIdx];
          } else {
            newVert.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); // Default
          }
        } else {
          
          newVert.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f); // Default
        }

        outVertices.push_back(newVert);
        unsigned int newIndex = static_cast<unsigned int>(outVertices.size() - 1);
        uniqueVertices[vertexString] = newIndex;
        faceIndices.push_back(newIndex);
      }
      catch (const std::invalid_argument& e) {
        std::string e_what(e.what());
        std::wstring w_msg(L"Indice no numerico: ");
        w_msg.append(e_what.begin(), e_what.end());
        ERROR(L"ModelLoader", L"processFace", w_msg.c_str());
        continue;
      }
      catch (const std::out_of_range& e) {
        std::string e_what(e.what());
        std::wstring w_msg(L"Indice fuera de rango: ");
        w_msg.append(e_what.begin(), e_what.end());
        ERROR(L"ModelLoader", L"processFace", w_msg.c_str());
        continue;
      }
    }
  }

  if (faceIndices.size() == 3) {
    outIndices.push_back(faceIndices[0]);
    outIndices.push_back(faceIndices[1]);
    outIndices.push_back(faceIndices[2]);
  }
  else if (faceIndices.size() == 4) {
    outIndices.push_back(faceIndices[0]);
    outIndices.push_back(faceIndices[1]);
    outIndices.push_back(faceIndices[2]);
    outIndices.push_back(faceIndices[0]);
    outIndices.push_back(faceIndices[2]);
    outIndices.push_back(faceIndices[3]);
  }
}

bool
ModelLoader::loadFromFile(const std::string& filename, 
                          MeshComponent& outMesh) {

  std::vector<XMFLOAT3> temp_positions;
  std::vector<XMFLOAT2> temp_texcoords;
  std::vector<XMFLOAT3> temp_normals; 
  std::vector<SimpleVertex> outVertices;
  std::vector<unsigned int> outIndices;
  std::unordered_map<std::string, unsigned int> uniqueVertices{};

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::wstring w_filename(filename.begin(), filename.end());
    std::wstring w_msg = L"No se pudo abrir el archivo: " + w_filename;
    ERROR(L"ModelLoader", L"loadFromFile", w_msg.c_str());
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string prefix;
    ss >> prefix; 

    if (prefix == "v") {
      XMFLOAT3 pos;
      if (ss >> pos.x >> pos.y >> pos.z) {
        temp_positions.push_back(pos);
      }
    }
    else if (prefix == "vt") {
      XMFLOAT2 tex;
      if (ss >> tex.x >> tex.y) {
        tex.y = 1.0f - tex.y; // Invertir V
        temp_texcoords.push_back(tex);
      }
    }
    else if (prefix == "vn") {
       XMFLOAT3 norm;
       if(ss >> norm.x >> norm.y >> norm.z) {
         temp_normals.push_back(norm); 
       }
    }
    else if (prefix == "f") {
      std::vector<std::string> faceData;
      std::string vertexString;
      while (ss >> vertexString) {
        faceData.push_back(vertexString);
      }
      
      if (faceData.size() >= 3) {
        processFace(faceData,
                    uniqueVertices,
                    outVertices,
                    outIndices,
                    temp_positions,
                    temp_texcoords,
                    temp_normals); 
      }
    }
  }

  file.close();

  outMesh.m_name = filename;
  outMesh.m_vertex = std::move(outVertices);
  outMesh.m_index = std::move(outIndices);
  outMesh.m_numVertex = static_cast<int>(outMesh.m_vertex.size());
  outMesh.m_numIndex = static_cast<int>(outMesh.m_index.size());

  if (outMesh.m_numVertex == 0 || outMesh.m_numIndex == 0) {
     std::wstring w_filename(filename.begin(), filename.end());
     std::wstring w_msg = L"Modelo vacio o malformado: " + w_filename;
     ERROR(L"ModelLoader", L"loadFromFile", w_msg.c_str());
     return false;
  }

  std::wostringstream wss;
  std::wstring w_filename(filename.begin(), filename.end());
  wss << L"Exito. " << w_filename << L" [V: " 
    << outMesh.m_numVertex << L", I: " << outMesh.m_numIndex << L"]";
  MESSAGE(L"ModelLoader", L"loadFromFile", wss.str().c_str());
  
  return true;
}