cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Test)

set(ENGINE_SOURCES 
    src/DummyEngine/Addition/base.h    
    src/DummyEngine/Addition/de_lib.h    

    src/DummyEngine/Core/ECS/component_array.hpp
    src/DummyEngine/Core/ECS/component_manager.hpp
    src/DummyEngine/Core/ECS/entity.hpp
    src/DummyEngine/Core/ECS/entity_manager.hpp
    src/DummyEngine/Core/ECS/system.hpp
    src/DummyEngine/Core/ECS/system_manager.hpp
    src/DummyEngine/Core/ECS/transformation.cpp

    src/DummyEngine/Core/Initializer/initializer.cpp

    src/DummyEngine/Addition/some_funcs.cpp

    src/DummyEngine/Core/Objects/LightSources/point_light.cpp
    src/DummyEngine/Core/Objects/LightSources/spot_light.cpp
    src/DummyEngine/Core/Objects/LightSources/directional_light.cpp
    src/DummyEngine/Core/Objects/LightSources/light_manager.cpp
    src/DummyEngine/Core/Objects/Cameras/fps_camera.cpp

    src/DummyEngine/Core/Rendering/Renderer/vertex_array.cpp
    src/DummyEngine/Core/Rendering/Renderer/vertex_buffer.cpp
    src/DummyEngine/Core/Rendering/Renderer/renderer.cpp
    src/DummyEngine/Core/Rendering/Renderer/render_data.cpp
    src/DummyEngine/Core/Rendering/Renderer/shader.cpp
    src/DummyEngine/Core/Rendering/Renderer/texture.cpp

    src/DummyEngine/Core/Rendering/RendererOpenGL/gl_vao.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/gl_vbo.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/gl_shader.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/gl_texture.cpp
    src/DummyEngine/Core/Rendering/RendererOpenGL/gl_render_api.cpp

    src/DummyEngine/Core/Input/input.cpp

    src/DummyEngine/Core/Wrappings/window.cpp
    src/DummyEngine/Core/Wrappings/application.cpp

    src/DummyEngine/ToolBox/Dev/logger.cpp
    src/DummyEngine/ToolBox/Dev/frame_time_reader.cpp
    src/DummyEngine/ToolBox/Loaders/model_loader.cpp
    src/DummyEngine/ToolBox/Loaders/texture_loader.cpp
)

add_library(my_lib
   ${ENGINE_SOURCES}
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

target_include_directories(my_lib
    PUBLIC libs/glfw/include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/glad/include
    PUBLIC libs/glm
    PUBLIC libs/assimp/include
    PUBLIC libs/stb_image
    PUBLIC src
)

target_link_directories(my_lib
    PUBLIC libs/glad/src
    PUBLIC libs/glfw/src
    PUBLIC libs/assimp/lib
)

target_link_libraries(my_lib
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
)

ADD_CUSTOM_COMMAND(TARGET my_lib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/ASSIMP/lib/libassimp.dll ${PROJECT_BINARY_DIR}/.)

add_executable(New_Model src/Mains/new_model.cpp)
# add_executable(Test src/Mains/test.cpp)
# add_executable(First src/Mains/first.cpp)
# add_executable(Windows src/Mains/multiple_windows.cpp)
# add_executable(ECS src/Mains/ECS.cpp)
# add_executable(Hello_window src/Mains/hello_window.cpp)

target_link_libraries(New_Model PUBLIC my_lib)
# target_link_libraries(Test PUBLIC my_lib)
# target_link_libraries(First PUBLIC my_lib)
# target_link_libraries(Windows PUBLIC my_lib)
# target_link_libraries(ECS PUBLIC my_lib)
# target_link_libraries(Hello_window PUBLIC my_lib)

