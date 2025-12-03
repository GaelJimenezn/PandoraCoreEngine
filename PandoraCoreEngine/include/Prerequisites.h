#pragma once
// Standard Libraries
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread> 
#include <memory>
#include <unordered_map>
#include <type_traits>

// DirectX Libraries
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"
#include "resource.h"

// Third Party & Utility Libraries
#include "EngineUtilities/Vectors/Vector2.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "EngineUtilities\Memory\TSharedPointer.h"
#include "EngineUtilities\Memory\TWeakPointer.h"
#include "EngineUtilities\Memory\TStaticPtr.h"
#include "EngineUtilities\Memory\TUniquePtr.h"

// MACROS

/**
 * @def SAFE_RELEASE
 * @brief Safely releases a COM object and sets its pointer to nullptr.
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

/**
 * @def MESSAGE
 * @brief Logs a debug message indicating resource creation state.
 */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : "\
       << "[CREATION OF RESOURCE " << ": "   \
       << state << "] \n";                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

/**
 * @def ERROR
 * @brief Logs a formatted error message to the Visual Studio debug console.
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

/**
 * @struct SimpleVertex
 * @brief Basic vertex structure containing position and texture coordinates.
 */
struct
SimpleVertex {
  XMFLOAT3 Pos; ///< Position in 3D space.
  XMFLOAT2 Tex; ///< UV texture coordinates.
};

/**
 * @struct CBNeverChanges
 * @brief Constant Buffer for data that updates rarely (e.g., View Matrix).
 */
struct
CBNeverChanges {
  XMMATRIX mView; ///< View transformation matrix.
};

/**
 * @struct CBChangeOnResize
 * @brief Constant Buffer for data updating on window resize (Projection).
 */
struct
CBChangeOnResize {
  XMMATRIX mProjection; ///< Projection matrix.
};

/**
 * @struct CBChangesEveryFrame
 * @brief Constant Buffer for per-frame data (World Matrix, Materials).
 */
struct
CBChangesEveryFrame {
  XMMATRIX mWorld;      ///< World transformation matrix.
  XMFLOAT4 vMeshColor;  ///< Base mesh color/tint.
};

/**
 * @enum ExtensionType
 * @brief Supported image file extensions.
 */
enum
ExtensionType {
  DDS = 0, ///< DirectDraw Surface.
  PNG = 1, ///< Portable Network Graphics.
  JPG = 2  ///< JPEG Image.
};

/**
 * @enum ShaderType
 * @brief Identifies the pipeline stage of a shader.
 */
enum
ShaderType {
  VERTEX_SHADER = 0, ///< Vertex Processing stage.
  PIXEL_SHADER = 1   ///< Pixel/Fragment Processing stage.
};

/**
 * @enum ComponentType
 * @brief Types of components available in the ECS architecture.
 */
enum
ComponentType {
  NONE = 0,      ///< Unspecified component type.
  TRANSFORM = 1, ///< Transform component (Position, Rotation, Scale).
  MESH = 2,      ///< Mesh geometry component.
  MATERIAL = 3   ///< Rendering material component.
};