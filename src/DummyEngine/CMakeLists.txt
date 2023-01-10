set(SANDBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Sandbox)
set(EXTERNAL_LIBS_DIR libs)
set(DE_LIB_SOURCE_DIR .) 

set(ENGINE_SOURCES  
    Addition/GLDebug.cpp
    Addition/Assert.cpp
    Addition/Config.cpp
    Addition/Conversions.cpp
    Addition/FileDialogs.cpp
    Addition/UUID.cpp
    Addition/Random.cpp
    Addition/Timer.cpp

    Core/Application/Application.cpp
    Core/Application/ImGuiLayer.cpp
    Core/Application/ImGuiPanel.cpp
    Core/Application/ImGuiPanelStack.cpp
    Core/Application/Window.cpp
    Core/Application/Input.cpp

    Core/ECS/ComponentArray.hpp
    Core/ECS/ComponentManager.hpp
    Core/ECS/Entity.hpp
    Core/ECS/EntityManager.hpp
    Core/ECS/System.hpp
    Core/ECS/SystemManager.hpp
    Core/ECS/ECSStorage.hpp
    
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
  
    Core/Scripting/Compiler.cpp
    Core/Scripting/ScriptInstance.cpp
    Core/Scripting/ScriptManager.cpp

    ToolBox/Dev/Logger.cpp
    ToolBox/Dev/Profiler.cpp
    ToolBox/Loaders/ModelLoader.cpp
    ToolBox/Loaders/TextureLoader.cpp 
    ToolBox/Loaders/SceneLoader.cpp
    ToolBox/Editors/TextureEditor.cpp
)

add_subdirectory(Libs)
add_library(DummyEngineLib SHARED
   ${ENGINE_SOURCES}
)
target_compile_definitions(DummyEngineLib PUBLIC 
    DE_PROFILER_ENABLED=1
    DE_ENABLE_ASSERTS=1
)

get_target_property(DependenciesInclude Dependencies INCLUDE_DIRECTORIES)

target_include_directories(DummyEngineLib
    PUBLIC ../.
)
target_link_libraries(DummyEngineLib
    PUBLIC Dependencies    
)


# add_custom_command(TARGET DummyEngineLib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/lib/libassimp.dll  ${SANDBOX_DIR})