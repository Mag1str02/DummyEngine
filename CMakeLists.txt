cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Test)

add_subdirectory(./external/GLAD ${CMAKE_CURRENT_BINARY_DIR}/GLAD)

INCLUDE_DIRECTORIES(external/ASSIMP/include)
INCLUDE_DIRECTORIES(external/GLFW/include)
INCLUDE_DIRECTORIES(external/GLAD/include)
INCLUDE_DIRECTORIES(external/GLM/)

add_library(my_lib STATIC  
    ./external/STB_IMAGE/stb_image.cpp    

    ./src/Addition/Funcs/funcs.cpp

    ./src/Objects/BasicGeometry/Cube/cube.cpp
    ./src/Objects/Classes/WorldObject/world_object.cpp
    ./src/Objects/Classes/LightSources/DirectionalLight/directional_light.cpp
    ./src/Objects/Classes/LightSources/PointLight/point_light.cpp
    ./src/Objects/Classes/LightSources/SpotLight/spot_light.cpp
    ./src/Objects/Managers/LightManager/light_manager.cpp
    ./src/Objects/Managers/InputManager/input_manager.cpp
    ./src/Objects/Mesh/mesh.cpp
    ./src/Objects/Model/model.cpp
    ./src/Objects/TextureManager/texture_manager.cpp
    
    ./src/Wrapping/GLFWInitializer/glfw_initializer.cpp
    ./src/Wrapping/Shader/shader.cpp
    ./src/Wrapping/ShaderProgram/shader_program.cpp
    ./src/Wrapping/Texture/texture.cpp
    ./src/Wrapping/Camera/camera.cpp
    ./src/Wrapping/Logger/logger.cpp
    ./src/Wrapping/Window/window.cpp
)

ADD_CUSTOM_COMMAND(TARGET my_lib
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/ASSIMP/lib/libassimp.dll ${PROJECT_BINARY_DIR}/.
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/GLFW/lib/glfw3.dll ${PROJECT_BINARY_DIR}/.
)

set_target_properties(my_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MyLib)

add_executable(LightCasters
    ./src/Mains/light_casters.cpp
)

target_link_libraries(my_lib PRIVATE glad)
target_link_libraries(my_lib PRIVATE glfw3)
target_link_libraries(my_lib PRIVATE assimp)

target_link_libraries(LightCasters PRIVATE my_lib)
