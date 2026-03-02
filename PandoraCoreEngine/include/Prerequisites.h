#pragma once
// Librerias STD
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>
#include <memory>
#include <unordered_map>
#include <type_traits>
#include <array>

// Librerias DirectX
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"
#include "resource.h"

// Third Party Libraries
#include "EngineUtilities/Vectors/Vector2.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "EngineUtilities\Memory\TSharedPointer.h"
#include "EngineUtilities\Memory\TWeakPointer.h"
#include "EngineUtilities\Memory\TStaticPtr.h"
#include "EngineUtilities\Memory\TUniquePtr.h"

/** * @def SAFE_RELEASE(x)
 * @brief Libera un recurso de DirectX de forma segura y lo nulifica.
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

/** * @def MESSAGE(classObj, method, state)
 * @brief Registra un mensaje de creación de recurso en la consola de salida.
 */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " \
       << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

/** * @def ERROR(classObj, method, errorMSG)
 * @brief Registra un error crítico en la consola de depuración de Windows.
 */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------

/** @struct SimpleVertex
 * @brief Estructura básica de vértice para mallas 3D. */
struct SimpleVertex
{
  XMFLOAT3 Pos; /**< Posición en el espacio 3D. */
  XMFLOAT2 Tex; /**< Coordenadas de textura (UV). */
};

struct 
SkyboxVertex {
	float x,y,z;
};

/** @struct CBNeverChanges
 * @brief Constant Buffer para datos que no cambian tras la inicialización. */
struct CBNeverChanges
{
  XMMATRIX mView; /**< Matriz de vista de la cámara. */
};

struct CBSkybox
{
  XMMATRIX mviewProj;
};

/** @struct CBChangeOnResize
 * @brief Constant Buffer para datos que cambian al redimensionar la ventana. */
struct CBChangeOnResize
{
  XMMATRIX mProjection; /**< Matriz de proyección. */
};

/** @struct CBChangesEveryFrame
 * @brief Constant Buffer para datos que se actualizan en cada frame. */
struct CBChangesEveryFrame
{
  XMMATRIX mWorld;      /**< Matriz de mundo del objeto. */
  XMFLOAT4 vMeshColor;  /**< Color base de la malla. */
};

/** @enum ExtensionType
 * @brief Extensiones de archivos de imagen soportadas. */
enum ExtensionType {
  DDS = 0, /**< DirectDraw Surface. */
  PNG = 1, /**< Portable Network Graphics. */
  JPG = 2  /**< Joint Photographic Experts Group. */
};

/** @enum ShaderType
 * @brief Tipos de Shaders de DirectX soportados. */
enum ShaderType {
  VERTEX_SHADER = 0, /**< Procesamiento de vértices. */
  PIXEL_SHADER = 1   /**< Procesamiento de fragmentos/píxeles. */
};

/**
 * @enum ComponentType
 * @brief Tipos de componentes disponibles en el juego.
 */
enum
  ComponentType {
  NONE = 0,      ///< Tipo de componente no especificado.
  TRANSFORM = 1, ///< Componente de transformación (Pos/Rot/Scale).
  MESH = 2,      ///< Componente que contiene la geometría.
  MATERIAL = 3,  ///< Componente que define la apariencia visual.
  HIERARCHY = 4  ///< Componente para relaciones padre-hijo.
};