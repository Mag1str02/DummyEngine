cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)

project(DummyEngineLib)
    
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
    Addition/STBImplementation.cpp
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
  
    Core/Scripting/ScriptInstance.hpp
    # Core/Scripting/ScriptManager.cpp

    ToolBox/Dev/Logger.cpp
    ToolBox/Dev/Profiler.cpp
    ToolBox/Loaders/ModelLoader.cpp
    ToolBox/Loaders/TextureLoader.cpp 
    ToolBox/Loaders/SceneLoader.cpp
    ToolBox/Editors/TextureEditor.cpp
)

add_library(DummyEngineLib
   ${ENGINE_SOURCES}
)

target_compile_definitions(DummyEngineLib PUBLIC 
    DE_PROFILER_ENABLED=1
    DE_ENABLE_ASSERTS=1
)

add_subdirectory(libs/assimp)
add_subdirectory(libs/imgui)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
add_subdirectory(libs/yaml)

target_include_directories(DummyEngineLib
    PUBLIC ${EXTERNAL_LIBS_DIR}/glfw/include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/glad/include
    PUBLIC ${EXTERNAL_LIBS_DIR}/glm
    PUBLIC ${EXTERNAL_LIBS_DIR}/assimp/include
    PUBLIC ${EXTERNAL_LIBS_DIR}/stb_image   
    PUBLIC ${EXTERNAL_LIBS_DIR}/yaml/include
    PUBLIC ${EXTERNAL_LIBS_DIR}/imgui
    PUBLIC ${DE_LIB_SOURCE_DIR}
)

target_link_libraries(DummyEngineLib
    PUBLIC ImGui
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
    PUBLIC yaml-cpp
)

# add_custom_command(TARGET DummyEngineLib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/lib/libassimp.dll  ${SANDBOX_DIR})