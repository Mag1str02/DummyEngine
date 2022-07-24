cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Test)

set(ENGINE_SOURCES 
    src/DummyEngine/Addition/Base.h   
    src/DummyEngine/Addition/Config.cpp
    src/DummyEngine/Addition/DummyEngine.h    

    src/DummyEngine/Core/ECS/ComponentArray.hpp
    src/DummyEngine/Core/ECS/ComponentManager.hpp
    src/DummyEngine/Core/ECS/Entity.hpp
    src/DummyEngine/Core/ECS/EntityManager.hpp
    src/DummyEngine/Core/ECS/System.hpp
    src/DummyEngine/Core/ECS/SystemManager.hpp
    src/DummyEngine/Core/ECS/Transformation.cpp

    src/DummyEngine/Core/Initializer/Initializer.cpp

    src/DummyEngine/Core/Objects/LightSources/PointLight.cpp
    src/DummyEngine/Core/Objects/LightSources/SpotLight.cpp
    src/DummyEngine/Core/Objects/LightSources/DirectionalLight.cpp
    src/DummyEngine/Core/Objects/LightSources/LightManager.cpp
    src/DummyEngine/Core/Objects/Cameras/FPSCamera.cpp

    src/DummyEngine/Core/Rendering/Renderer/VertexArray.cpp
    src/DummyEngine/Core/Rendering/Renderer/VertexBuffer.cpp
    src/DummyEngine/Core/Rendering/Renderer/Renderer.cpp
    src/DummyEngine/Core/Rendering/Renderer/RenderStructs.cpp
    src/DummyEngine/Core/Rendering/Renderer/Shader.cpp
    src/DummyEngine/Core/Rendering/Renderer/Texture.cpp

    src/DummyEngine/Core/Rendering/RendererOpenGL/GLVertexArray.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/GLVertexBuffer.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/GLShader.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/GLRenderAPI.cpp

    src/DummyEngine/Core/Input/Input.cpp

    src/DummyEngine/Core/Wrappings/Window.cpp
    src/DummyEngine/Core/Wrappings/Application.cpp

    src/DummyEngine/ToolBox/Dev/Logger.cpp
    src/DummyEngine/ToolBox/Dev/FrameTimeReader.cpp
    src/DummyEngine/ToolBox/Loaders/ModelLoader.cpp
    src/DummyEngine/ToolBox/Loaders/TextureLoader.cpp
)

add_library(dummy_engine_lib
   ${ENGINE_SOURCES}
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

target_include_directories(dummy_engine_lib
    PUBLIC libs/glfw/include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/glad/include
    PUBLIC libs/glm
    PUBLIC libs/assimp/include
    PUBLIC libs/stb_image
    PUBLIC src
)

target_link_directories(dummy_engine_lib
    PUBLIC libs/glad/src
    PUBLIC libs/glfw/src
    PUBLIC libs/assimp/lib
)

target_link_libraries(dummy_engine_lib
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
)

ADD_CUSTOM_COMMAND(TARGET dummy_engine_lib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/ASSIMP/lib/libassimp.dll ${PROJECT_BINARY_DIR}/.)

add_executable(New_Model src/Mains/new_model.cpp)
# add_executable(Test src/Mains/test.cpp)
# add_executable(First src/Mains/first.cpp)
# add_executable(Windows src/Mains/multiple_windows.cpp)
# add_executable(ECS src/Mains/ECS.cpp)
# add_executable(Hello_window src/Mains/hello_window.cpp)

target_link_libraries(New_Model PUBLIC dummy_engine_lib)
# target_link_libraries(Test PUBLIC dummy_engine_lib)
# target_link_libraries(First PUBLIC dummy_engine_lib)
# target_link_libraries(Windows PUBLIC dummy_engine_lib)
# target_link_libraries(ECS PUBLIC dummy_engine_lib)
# target_link_libraries(Hello_window PUBLIC dummy_engine_lib)

