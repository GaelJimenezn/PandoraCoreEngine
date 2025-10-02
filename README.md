🚀 Pandora Core Engine

Pandora Core Engine is a 3D rendering engine built from scratch in C++ using DirectX 11
Developed as part of the 3D Computer Graphics course (Class of 2026), it implements the key concepts of the real-time graphics pipeline

The engine can initialize a window, configure DirectX 11, manage presentation buffers, and render a rotating textured cube

✨ Key Features

DirectX 11 Abstraction: Clean, reusable C++ classes encapsulating low-level components

Minimal Rendering Pipeline:

Native window creation using Win32

Device & DeviceContext management

SwapChain setup

RenderTargetView & DepthStencilView handling

Geometry Rendering: Loads and renders a 3D cube using vertex and index buffers

Shaders & Texturing: Vertex and Pixel Shaders implemented in HLSL

MSAA Support: Multisample anti-aliasing for smoother visuals

🏛 Engine Architecture

The engine is designed with separation of responsibilities, where each class has a clear role in the rendering pipeline

<details> <summary>Click to expand the class table</summary>
Class	Description
Window	Handles the native Win32 window
Device	Factory for GPU resources (textures, buffers, shaders)
DeviceContext	Generates rendering commands and assigns resources to the pipeline
SwapChain	Manages the buffer chain for presentation
Texture	Manages 2D textures and shader resource views
RenderTargetView	Represents the color buffer for rendering the scene
DepthStencilView	Manages depth buffer to ensure correct occlusion
</details>
🎨 Rendering Pipeline

Each frame follows this sequence

<details> <summary>Click to expand rendering steps</summary>

Update: Transformation matrices and dynamic data

Clear Buffers: RenderTargetView and DepthStencilView

Assign Resources:

Input Layout, Vertex Buffer & Index Buffer

Vertex Shader & Pixel Shader

Constant Buffers

Textures & Samplers

Draw Call: DrawIndexed() to render the cube

Present: SwapChain.Present() to display the final frame

</details>
📊 Architecture Diagrams
1️⃣ Class Diagram
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

2️⃣ Rendering Pipeline Flow
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

⚙ How to Compile & Run
Prerequisites

Windows 10 or higher

Visual Studio 2010 or newer

DirectX SDK (June 2010)

Compilation Steps

Clone the repository

Ensure DirectX SDK paths are configured in Visual Studio

Open PandoraCoreEngine_2010.sln

Build in Debug or Release mode
