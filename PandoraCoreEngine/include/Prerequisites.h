#pragma once
/**
 * @file Prerequisites.h
 * @brief Includes essential headers, defines core structures, and macros.
 */

// STD Libraries
/** @name Standard Libraries
 * @{
 */
#include <windows.h>
#include <xnamath.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <fstream>        
#include <unordered_map> 
#include <stdexcept>
#include <map>
/** @} */

// DirectX Libraries
/** @name DirectX Libraries
 * @{
 */
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"
/** @} */

// MACROS
/**
 * @def SAFE_RELEASE(x)
 * @brief Safely releases a COM object and sets its pointer to nullptr.
 * @param x The COM object (e.g., ID3D11Buffer*) to release.
 */
#define SAFE_RELEASE(x) if(x != nullptr) { x->Release(); x = nullptr; }

/**
 * @def MESSAGE(classObj, method, state)
 * @brief Logs a standard creation/status message to the debug output.
 * @param classObj The name of the class (as a wide string).
 * @param method The name of the method (as a wide string).
 * @param state The status message to log (as a wide string).
 */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " \
   << ": " << state << "] \n";               \
   OutputDebugStringW( os_.str().c_str() );  \
}

/**
 * @def ERROR(classObj, method, errorMSG)
 * @brief Logs an error message to the debug output, safely 
 * enclosed in try-catch.
 * @param classObj The name of the class (as a wide string).
 * @param method The name of the method (as a wide string).
 * @param errorMSG The error description (as a wide string).
 */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
  try {                                                       \
    std::wostringstream os_;                                  \
    os_ << L"ERROR : " << classObj << L"::" << method         \
        << L" : " << errorMSG << L"\n";                       \
    OutputDebugStringW(os_.str().c_str());                    \
  } catch (...) {                                             \
    OutputDebugStringW(L"Failed to log error message.\n");    \
  }                                                           \
}

// Structures
/**
 * @struct SimpleVertex
 * @brief Defines the data structure for a single vertex.
 * @details Contains position, texture coordinate, and normal data.
 */
struct 
SimpleVertex {
  XMFLOAT3 Pos;    /**< 3D position of the vertex (x, y, z). */
  XMFLOAT2 Tex;    /**< 2D texture coordinate of the vertex (u, v). */
  XMFLOAT3 Normal; /**< 3D normal vector of the vertex (x, y, z). */
};

/**
 * @struct CBNeverChanges
 * @brief Constant buffer struct for data that rarely changes (e.g., View matrix).
 */
struct 
CBNeverChanges {
  XMMATRIX mView; /**< The view matrix, updated once per camera change. */
};

/**
 * @struct CBChangeOnResize
 * @brief Constant buffer struct for data that changes on window resize (e.g., Projection matrix).
 */
struct
CBChangeOnResize {
  XMMATRIX mProjection; /**< The projection matrix, updated on resize. */
};

/**
 * @struct CBChangesEveryFrame
 * @brief Constant buffer struct for data that changes every frame (e.g., World matrix).
 */
struct 
CBChangesEveryFrame {
  XMMATRIX mWorld;      /**< The world matrix for a specific object. */
  XMFLOAT4 vMeshColor;  /**< A color value applied to the mesh. */
};

/**
 * @enum ExtensionType
 * @brief Defines supported texture file extensions.
 */
enum 
ExtensionType {
  DDS = 0, /**< DirectDraw Surface (.dds) */
  PNG = 1, /**< Portable Network Graphics (.png) */
  JPG = 2  /**< Joint Photographic Experts Group (.jpg) */
};

/**
 * @enum ShaderType
 * @brief Defines the types of shaders that can be created.
 */
enum ShaderType {
  VERTEX_SHADER = 0, /**< Represents a Vertex Shader. */
  PIXEL_SHADER = 1   /**< Represents a Pixel Shader. */
};