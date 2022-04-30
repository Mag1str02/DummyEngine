cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Test)

add_subdirectory(./libs/GLAD ${CMAKE_CURRENT_BINARY_DIR}/GLAD)

INCLUDE_DIRECTORIES(libs/ASSIMP/include)
INCLUDE_DIRECTORIES(libs/GLFW/include)
INCLUDE_DIRECTORIES(libs/GLAD/include)
INCLUDE_DIRECTORIES(libs/GLM/)

add_library(my_lib STATIC  
    ./libs/STB_IMAGE/stb_image.cpp   

    ./src/ToolBox/Loaders/ModelLoader/model_loader.cpp
    ./src/ToolBox/Loaders/TextureLoader/texture_loader.cpp

    ./src/Core/Memory/MeshManager/mesh_manager.cpp
    ./src/Core/Memory/ModelManager/model_manager.cpp
    ./src/Core/Memory/TextureManager/texture_manager.cpp
    ./src/Core/Memory/VAO/vao.cpp

    ./src/Core/Objects/LightSources/PointLight/point_light.cpp
    ./src/Core/Objects/LightSources/SpotLight/spot_light.cpp
    ./src/Core/Objects/LightSources/DirectionalLight/directional_light.cpp
    ./src/Core/Objects/LightSources/LightManager/light_manager.cpp
    ./src/Core/Objects/Model/model.cpp
    ./src/Core/Objects/Position/position.cpp
    ./src/Core/Objects/Cameras/FPSCamera/fps_camera.cpp

    ./src/Core/Structs/Meshes/mesh.cpp
    ./src/Core/Structs/Textures/Texture2D/texture_2D.cpp

    ./src/ToolBox/Dev/Logger/logger.cpp
    ./src/Core/UserInput/InputManager/input_manager.cpp

    ./src/Core/Wrappings/ApplicationWindow/application_window.cpp
    ./src/Core/Wrappings/Shader/shader.cpp
    ./src/Core/Wrappings/ShaderProgram/shader_program.cpp
)

ADD_CUSTOM_COMMAND(TARGET my_lib
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/ASSIMP/lib/libassimp.dll ${PROJECT_BINARY_DIR}/.
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/lib/glfw3.dll ${PROJECT_BINARY_DIR}/.
)

set_target_properties(my_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MyLib)

add_executable(First
    ./src/Mains/first.cpp
)

add_executable(Model
    ./src/Mains/model.cpp
)

add_executable(Windows
    ./src/Mains/multiple_windows.cpp
)

target_link_libraries(my_lib PRIVATE glad)
target_link_libraries(my_lib PRIVATE glfw3)
target_link_libraries(my_lib PRIVATE assimp)

target_link_libraries(First PRIVATE my_lib)
target_link_libraries(Model PRIVATE my_lib)
target_link_libraries(Windows PRIVATE my_lib)
