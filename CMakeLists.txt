cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Test)

add_library(my_lib
    ./src/Engine/Core/ECS/Component/component_array.hpp
    ./src/Engine/Core/ECS/Component/component_manager.hpp
    ./src/Engine/Core/ECS/Entity/entity.hpp
    ./src/Engine/Core/ECS/Entity/entity_manager.hpp
    ./src/Engine/Core/ECS/System/system.hpp
    ./src/Engine/Core/ECS/System/system_manager.hpp
    ./src/Engine/Core/ECS/BasicComponents/Transformation/transformation.cpp

    ./src/Engine/Core/Initializer/initializer.cpp

    ./src/Engine/Addition/some_funcs.cpp

    ./src/Engine/Core/Objects/LightSources/PointLight/point_light.cpp
    ./src/Engine/Core/Objects/LightSources/SpotLight/spot_light.cpp
    ./src/Engine/Core/Objects/LightSources/DirectionalLight/directional_light.cpp
    ./src/Engine/Core/Objects/LightSources/LightManager/light_manager.cpp
    ./src/Engine/Core/Objects/Cameras/FPSCamera/fps_camera.cpp

    ./src/Engine/Core/Rendering/RenderData/render_data.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/Buffers/VAO/gl_vao.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/Buffers/EBO/gl_ebo.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/Buffers/VBO/gl_vbo.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/Shader/gl_shader.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/RenderStructs/gl_render_structs.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/ShaderProgram/gl_shader_program.cpp    
    ./src/Engine/Core/Rendering/RendererOpenGL/Textures/Texture2D/gl_texture_2D.cpp
    ./src/Engine/Core/Rendering/RendererOpenGL/Renderer/gl_renderer.cpp

    ./src/Engine/Core/Input/input.cpp

    ./src/Engine/Core/Wrappings/Window/window.cpp
    ./src/Engine/Core/Wrappings/Application/application.cpp

    ./src/Engine/ToolBox/Dev/Logger/logger.cpp
    ./src/Engine/ToolBox/Dev/FrameTimeReader/frame_time_reader.cpp
    ./src/Engine/ToolBox/Loaders/ModelLoader/model_loader.cpp
    ./src/Engine/ToolBox/Loaders/TextureLoader/texture_loader.cpp
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

target_include_directories(my_lib
    PUBLIC libs/glfw/include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/glad/include
    PUBLIC libs/glm
    PUBLIC libs/assimp/include
    PUBLIC libs/stb_image
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

add_executable(Test ./src/Mains/test.cpp)
# add_executable(New_Model ./src/Mains/new_model.cpp)
# add_executable(First ./src/Mains/first.cpp)
# add_executable(Windows ./src/Mains/multiple_windows.cpp)
# add_executable(ECS ./src/Mains/ECS.cpp)
# add_executable(Hello_window ./src/Mains/hello_window.cpp)

target_link_libraries(Test PUBLIC my_lib)
# target_link_libraries(New_Model PUBLIC my_lib)
# target_link_libraries(First PUBLIC my_lib)
# target_link_libraries(Windows PUBLIC my_lib)
# target_link_libraries(ECS PUBLIC my_lib)
# target_link_libraries(Hello_window PUBLIC my_lib)

