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
