cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)

project(DummyEngineLib)

set(ENGINE_SOURCES  
    Addition/Assert.cpp
    Addition/Config.cpp
    Addition/UUID.cpp

    Core/ECS/ComponentArray.hpp
    Core/ECS/ComponentManager.hpp
    Core/ECS/Entity.hpp
    Core/ECS/EntityManager.hpp
    Core/ECS/System.hpp
    Core/ECS/SystemManager.hpp
    Core/ECS/ECSStorage.hpp

    Core/Initializer/Initializer.cpp
    
    Core/Scene/Components.cpp
    Core/Scene/Scene.cpp

    Core/Objects/LightSources/PointLight.cpp
    Core/Objects/LightSources/SpotLight.cpp
    Core/Objects/LightSources/DirectionalLight.cpp
    Core/Objects/LightSources/LightManager.cpp
    Core/Objects/Cameras/FPSCamera.cpp

    Core/Rendering/Renderer/VertexArray.cpp
    Core/Rendering/Renderer/VertexBuffer.cpp
    Core/Rendering/Renderer/Renderer.cpp
    Core/Rendering/Renderer/RenderStructs.cpp
    Core/Rendering/Renderer/Shader.cpp
    Core/Rendering/Renderer/Texture.cpp

    Core/Rendering/RendererOpenGL/GLVertexArray.cpp
    Core/Rendering/RendererOpenGL/GLVertexBuffer.cpp
    Core/Rendering/RendererOpenGL/GLShader.cpp
    Core/Rendering/RendererOpenGL/GLTexture.cpp
    Core/Rendering/RendererOpenGL/GLRenderAPI.cpp

    Core/Input/Input.cpp

    Core/Wrappings/Window.cpp
    Core/Wrappings/Application.cpp

    ToolBox/Dev/Logger.cpp
    ToolBox/Dev/FrameTimeReader.cpp
    ToolBox/Loaders/ModelLoader.cpp
    ToolBox/Loaders/TextureLoader.cpp
)

add_library(DummyEngineLib
   ${ENGINE_SOURCES}
)

target_compile_definitions(DummyEngineLib PUBLIC 
 DE_ENABLE_FRAME_TIME_READER=0
 DE_ENABLE_ASSERTS=1
 )

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

target_include_directories(DummyEngineLib
    PUBLIC libs/glfw/include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/glad/include
    PUBLIC libs/glm
    PUBLIC libs/assimp/include
    PUBLIC libs/stb_image
    PUBLIC .
)

target_link_directories(DummyEngineLib
    PUBLIC libs/glad/src
    PUBLIC libs/glfw/src
    PUBLIC libs/assimp/lib
)

target_link_libraries(DummyEngineLib
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
)

add_custom_command(TARGET DummyEngineLib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/Assimp/lib/libassimp.dll  ${CMAKE_CURRENT_BINARY_DIR}/../../../Sandbox)
