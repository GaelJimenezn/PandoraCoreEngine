Pandora Core Engine

Pandora Core Engine is a 3D rendering engine built from scratch in C++ using the DirectX 11 API. This project was developed as part of the 3D Computer Graphics course (Class of 2026) and serves as a practical implementation of the key concepts of the real-time graphics pipeline.

The engine can initialize a window, configure DirectX 11, manage presentation buffers, and render a simple 3D scene with a rotating textured cube.

Key Features

DirectX 11 Abstraction: Low-level components are encapsulated in clean, reusable C++ classes.

Minimal Rendering Pipeline:

Native window creation with Win32.

Management of Device and DeviceContext.

SwapChain setup.

Use of RenderTargetView and DepthStencilView.

Geometry Rendering: Loads and draws a 3D cube using vertex and index buffers.

Shaders and Texturing: Vertex and Pixel Shaders implemented in HLSL.

MSAA Support: Multisample anti-aliasing for improved visual quality.

Engine Architecture

The engine is designed with separation of responsibilities, where each class has a clear role within the rendering pipeline:

Class	Description
Window	Handles the native Win32 window.
Device	Factory for GPU resources (textures, buffers, shaders).
DeviceContext	Generates rendering commands and assigns resources to the pipeline.
SwapChain	Manages the buffer chain for presentation.
Texture	Manages 2D texture resources and shader resource views.
RenderTargetView	Represents the color buffer where the scene is drawn.
DepthStencilView	Manages the depth buffer to ensure proper occlusion.
Rendering Pipeline

Each frame follows these steps:

Update: Transformation matrices and other dynamic data.

Clear Buffers: RenderTargetView and DepthStencilView.

Resource Assignment:

Input Layout, Vertex Buffer, and Index Buffer.

Vertex Shader and Pixel Shader.

Constant Buffers.

Textures and Samplers.

Draw Call: DrawIndexed() to render the cube.

Presentation: SwapChain.Present() to display the final frame.

Architecture Diagrams
1. Class Diagram

This diagram shows the main classes and their relationships.

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

2. Rendering Pipeline Flow Diagram

This diagram describes the dynamic sequence of operations for each frame.

graph TD
    subgraph "Initialization Phase (InitDevice)"
        A[Create Window] --> B(Create SwapChain)
        B --> C{Create Device and DeviceContext}
        C --> D[Create RenderTargetView from Back Buffer]
        C --> E[Create Texture for Depth Buffer]
        E --> F[Create DepthStencilView]
        C --> G[Compile Shaders]
        C --> H[Create Vertex/Index Buffers]
        C --> I[Create Constant Buffers]
        C --> J[Load Texture from File]
    end

    subgraph "Rendering Loop (Render)"
        K[Start Frame] --> L[Update Constant Buffers (Matrices, Color, etc.)]
        L --> M[Clear RenderTargetView and DepthStencilView]
        M --> N[Assign Resources to Pipeline]
        N --> O[IA: Input Assembler<br/>- Vertex/Index Buffers<br/>- Input Layout]
        O --> P[VS: Vertex Shader<br/>- Constant Buffers]
        P --> Q[PS: Pixel Shader<br/>- Constant Buffers<br/>- Texture<br/>- Sampler]
        Q --> R[OM: Output Merger<br/>- RenderTargetView<br/>- DepthStencilView]
        R --> S[DeviceContext.drawIndexed()]
        S --> T[SwapChain.present()]
        T --> K
    end

    J --> Q
    I --> P
    H --> O
    G --> P
    G --> Q
    F --> R
    D --> R

How to Compile and Run
Prerequisites

Windows 10 or higher

Visual Studio 2010 or newer

DirectX SDK (June 2010)

Compilation

Clone the repository

Ensure DirectX SDK paths are correctly configured in Visual Studio

Open PandoraCoreEngine_2010.sln

Build in Debug or Release
