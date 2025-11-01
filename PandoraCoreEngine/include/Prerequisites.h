#pragma once
/**
 * @file Prerequisites.h
 * @brief Includes essential headers, defines core structures, and macros.
 */

// STD Libraries
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

// DirectX Libraries
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"

// MACROS
#define SAFE_RELEASE(x) if(x != nullptr) { x->Release(); x = nullptr; }

#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " \
   << ": " << state << "] \n";               \
   OutputDebugStringW( os_.str().c_str() );  \
}

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
struct 
SimpleVertex {
  XMFLOAT3 Pos; /**< 3D position of the vertex (x, y, z). */
  XMFLOAT2 Tex; /**< 2D texture coordinate of the vertex (u, v). */
};

struct 
CBNeverChanges {
  XMMATRIX mView; /**< The view matrix, updated once per camera change. */
};

struct
CBChangeOnResize {
  XMMATRIX mProjection; /**< The projection matrix, updated on resize. */
};

struct 
CBChangesEveryFrame {
  XMMATRIX mWorld;      /**< The world matrix for a specific object. */
  XMFLOAT4 vMeshColor;  /**< A color value applied to the mesh. */
};

enum 
ExtensionType {
  DDS = 0,
  PNG = 1,
  JPG = 2
};

enum ShaderType {
  VERTEX_SHADER = 0,
  PIXEL_SHADER = 1
};