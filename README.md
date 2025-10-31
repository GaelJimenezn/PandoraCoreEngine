
# 🎮 Pandora Core Engine ✨

<p align="center">
  <img src="URL_TO_YOUR_DEMO_GIF_OR_IMAGE.gif" alt="Pandora Core Engine Demo" width="600"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B" alt="Language C++">
  <img src="https://img.shields.io/badge/API-DirectX_11-0078D7?style=for-the-badge&logo=windows" alt="API DirectX 11">
  <img src="https://img.shields.io/badge/Platform-Windows-0078D7?style=for-the-badge&logo=windows" alt="Platform Windows">
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" alt="License MIT">
</p>

Pandora Core Engine is a 3D rendering engine built from scratch in C++ using DirectX 11. Created for the 3D Computer Graphics course (Class of 2026), this engine implements a real-time rendering pipeline to display a rotating, textured cube.

## 📜 Table of Contents
* [🚀 Key Features](#-key-features)
* [🏗️ Engine Architecture](#️-engine-architecture)
* [🎨 Rendering Pipeline](#-rendering-pipeline)
* [📊 Architecture Diagrams](#-architecture-diagrams)
* [⚙️ Compile & Run](#️-compile--run)

## 🚀 Key Features

* **DirectX 11 Abstraction:** Clean and reusable C++ classes encapsulating low-level API components.
* **Minimal Rendering Pipeline:**
    * Native Win32 window creation.
    * `Device` & `DeviceContext` management.
    * `SwapChain` setup.
    * `RenderTargetView` & `DepthStencilView` handling.
* **Geometry Rendering:** Renders a 3D cube using vertex and index buffers.
* **Shaders & Texturing:** Vertex and Pixel Shaders written in HLSL.
* **MSAA Support:** Multisample anti-aliasing for smoother visuals.

## 🏗️ Engine Architecture

The engine uses the separation of responsibilities principle, giving each class a clear and defined role.

| Class              | Description                                                      |
| ------------------ | ---------------------------------------------------------------- |
| `Window`           | Manages the native Win32 window (HWND).                          |
| `Device`           | Factory for GPU resources (textures, buffers, shaders).          |
| `DeviceContext`    | Sends commands to the GPU and binds resources to the pipeline.   |
| `SwapChain`        | Manages the buffer chain for presentation to the screen.         |
| `Texture`          | Represents 2D texture resources and their shader views.          |
| `RenderTargetView` | The color buffer where the scene is rendered to.                 |
| `DepthStencilView` | The depth buffer used for correct object occlusion.              |


## 🎨 Rendering Pipeline

Every frame follows this execution flow:

1.  **Update:** Transformation matrices and other dynamic data are updated.
2.  **Clear Buffers:** The `RenderTargetView` and `DepthStencilView` are cleared.
3.  **Bind Resources:**
    * Input Layout, Vertex & Index Buffers.
    * Vertex Shader & Pixel Shader.
    * Constant Buffers (data for shaders).
    * Textures & Samplers.
4.  **Draw Call:** `DrawIndexed()` is invoked to render the geometry.
5.  **Present:** The `SwapChain` buffers are swapped to display the new frame.

## 📊 Architecture Diagrams

#### 1️⃣ Class Diagram
```mermaid
classDiagram
    direction LR
    class PandoraCoreEngine {
        +wWinMain()
        +InitDevice()
        +Render()
        +CleanupDevice()
    }

    class Window { +HWND m_hWnd +init() }
    class Device { +ID3D11Device* m_device +createTexture2D() +createBuffer() +createShader() }
    class DeviceContext { +ID3D11DeviceContext* m_deviceContext +drawIndexed() +updateSubresource() }
    class SwapChain { +IDXGISwapChain* m_swapChain +present() }
    class Texture { +ID3D11Texture2D* m_texture }
    class RenderTargetView { +ID3D11RenderTargetView* m_renderTargetView }
    class DepthStencilView { +ID3D11DepthStencilView* m_depthStencilView }

    PandoraCoreEngine o-- Window
    PandoraCoreEngine o-- Device
    PandoraCoreEngine o-- DeviceContext
    PandoraCoreEngine o-- SwapChain
    PandoraCoreEngine o-- RenderTargetView
    PandoraCoreEngine o-- DepthStencilView
    PandoraCoreEngine o-- Texture

    SwapChain ..> Window : uses
    SwapChain ..> Device : creates
    SwapChain ..> DeviceContext : creates
    SwapChain ..> Texture : fills
    RenderTargetView ..> Device : created by
    RenderTargetView ..> Texture : uses
    DepthStencilView ..> Device : created by
    DepthStencilView ..> Texture : uses
=======
Pandora Core Engine
Pandora Core Engine es un motor de renderizado 3D fundamental construido desde cero utilizando C++ y la API de DirectX 11. Este proyecto fue desarrollado como parte de la materia "Gráficas Computacionales 3D" (Generación 2026) y sirve como una implementación práctica de los conceptos clave del pipeline de gráficos en tiempo real.

El motor es capaz de inicializar una ventana, configurar un dispositivo de DirectX 11, gestionar los búferes de presentación y renderizar una escena 3D simple con un cubo texturizado que rota.

Características Principales
Abstracción de DirectX 11: El código encapsula los componentes de bajo nivel de DirectX en clases C++ limpias y reutilizables.

Pipeline de Renderizado Mínimo: Implementa un pipeline gráfico funcional, incluyendo:

Creación de ventana nativa con la API de Win32.

Gestión del Device para la creación de recursos y DeviceContext para la ejecución de comandos.

Configuración del SwapChain para la presentación en pantalla.

Uso de RenderTargetView y DepthStencilView para gestionar los búferes de color y profundidad.

Renderizado de Geometría: Carga y dibuja un cubo 3D utilizando buffers de vértices e índices.

Shaders y Texturizado: Utiliza Vertex y Pixel Shaders escritos en HLSL para aplicar transformaciones y texturas a los objetos en la escena.

Soporte para MSAA: Configura el multisampling anti-aliasing para mejorar la calidad visual y suavizar los bordes de la geometría.

Arquitectura del Motor
La arquitectura del motor se basa en la separación de responsabilidades, donde cada clase tiene un propósito bien definido dentro del pipeline de renderizado:

Window: Encapsula la creación y el manejo de la ventana nativa de Win32, que sirve como superficie principal para el renderizado.

Device: Actúa como una fábrica para todos los recursos de la GPU (texturas, buffers, shaders, etc.). Envuelve la interfaz ID3D11Device.

DeviceContext: Envuelve ID3D11DeviceContext y se utiliza para generar los comandos de renderizado, asignar recursos al pipeline y ejecutar las llamadas de dibujado.

SwapChain: Gestiona la cadena de búferes (front y back buffer) utilizada para mostrar las imágenes renderizadas en la pantalla.

Texture: Administra los recursos de texturas 2D y sus vistas (ShaderResourceView) para que puedan ser utilizadas por los shaders.

RenderTargetView: Representa el búfer (generalmente el back buffer) en el que se dibuja la escena.

DepthStencilView: Gestiona el búfer de profundidad para asegurar que la geometría se dibuje con la oclusión correcta.

Pipeline de Renderizado Implementado
El flujo de renderizado en cada fotograma sigue estos pasos:

Actualización: Se actualizan las matrices de transformación (Mundo, Vista, Proyección) y otros datos dinámicos, como el color del objeto.

Limpieza de Buffers: Se limpian el RenderTargetView a un color de fondo y el DepthStencilView para iniciar el frame con un estado limpio.

Asignación de Recursos: Se asignan al pipeline los recursos necesarios para el dibujado:

Input Layout, Vertex Buffer e Index Buffer.

Vertex Shader y Pixel Shader.

Constant Buffers con las matrices actualizadas.

Texturas y Samplers.

Llamada de Dibujado: Se invoca a DrawIndexed() para renderizar la geometría del cubo.

Presentación: Se llama a Present() en el SwapChain para intercambiar el back buffer con el front buffer, mostrando la imagen final en la pantalla.

Cómo Compilar y Ejecutar
Prerrequisitos
Windows 10 o superior.

Visual Studio 2010 o una versión más reciente.

DirectX SDK (Junio 2010), ya que el proyecto utiliza D3DX11.

Compilación
Clonar el repositorio.

Asegurarse de que las rutas del DirectX SDK estén correctamente configuradas en las propiedades del proyecto en Visual Studio (Include y Library Directories).

Abrir el archivo de solución PandoraCoreEngine_2010.sln.

Compilar el proyecto en la configuración deseada (Debug o Release).
