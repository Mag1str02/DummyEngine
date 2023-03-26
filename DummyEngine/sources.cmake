
set(ENGINE_SOURCES  
Utils/GLDebug.cpp
Utils/Assert.cpp
Utils/Config.cpp
Utils/Conversions.cpp
Utils/UUID.cpp
Utils/Random.cpp
Utils/Timer.cpp
Utils/StringOperations.cpp

Core/Application/Application.cpp
Core/Application/ImGuiLayer.cpp
Core/Application/ImGuiPanel.cpp
Core/Application/ImGuiPanelStack.cpp
Core/Application/Window.cpp
Core/Application/Input.cpp
Core/Application/Initializer.cpp

Core/ECS/ECS.cpp

Core/Scene/Components.cpp
Core/Scene/Scene.cpp
Core/Scene/SceneHierarchy.cpp
Core/Scene/SceneRenderData.cpp

Core/Objects/LightSources/LightSource.cpp
Core/Objects/Cameras/FPSCamera.cpp

Core/Rendering/Renderer/BufferLayout.cpp
Core/Rendering/Renderer/Context.cpp
Core/Rendering/Renderer/CubeMap.cpp
Core/Rendering/Renderer/FrameBuffer.cpp
Core/Rendering/Renderer/LocalBuffer.cpp
Core/Rendering/Renderer/VertexArray.cpp
Core/Rendering/Renderer/VertexBuffer.cpp
Core/Rendering/Renderer/Renderer.cpp
Core/Rendering/Renderer/RenderStructs.cpp
Core/Rendering/Renderer/Shader.cpp
Core/Rendering/Renderer/Texture.cpp
Core/Rendering/Renderer/UniformBuffer.cpp

Core/Rendering/RendererOpenGL/GLContext.cpp
Core/Rendering/RendererOpenGL/GLCubeMap.cpp
Core/Rendering/RendererOpenGL/GLFrameBuffer.cpp
Core/Rendering/RendererOpenGL/GLUtils.cpp
Core/Rendering/RendererOpenGL/GLVertexArray.cpp
Core/Rendering/RendererOpenGL/GLVertexBuffer.cpp
Core/Rendering/RendererOpenGL/GLShader.cpp
Core/Rendering/RendererOpenGL/GLTexture.cpp
Core/Rendering/RendererOpenGL/GLUniformBuffer.cpp
Core/Rendering/RendererOpenGL/GLRenderAPI.cpp

Core/ResourceManaging/AssetManager.cpp
Core/ResourceManaging/RawData.cpp
Core/ResourceManaging/ResourceManager.cpp

Core/Scripting/Script.cpp
Core/Scripting/ScriptEngine.cpp
Core/Scripting/ScriptClass.cpp

ToolBox/Dev/Logger.cpp
ToolBox/Dev/Profiler.cpp
ToolBox/Loaders/ModelLoader.cpp
ToolBox/Loaders/TextureLoader.cpp 
ToolBox/Loaders/SceneLoader.cpp
ToolBox/Editors/TextureEditor.cpp
)

if(${CMAKE_PLATFORM} STREQUAL "WINDOWS")
set(PLATFORM_SOURCES
    Platform/Windows/Utils/FileSystem.cpp
    Platform/Windows/Utils/StackTrace.cpp
    Platform/Windows/Core/Scripting/SharedObject.cpp
)
endif()
if(${CMAKE_PLATFORM} STREQUAL "LINUX")
set(PLATFORM_SOURCES)
endif()
if(${CMAKE_PLATFORM} STREQUAL "MACOS")
set(PLATFORM_SOURCES)
endif()