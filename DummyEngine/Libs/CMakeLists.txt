cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ASSIMP)
add_subdirectory(GLFW)
add_subdirectory(GLAD)
add_subdirectory(YAML)
add_subdirectory(NFD)



set(IMGUI_INCLUDES
	IMGUI/imgui.h
	IMGUI/backends/imgui_impl_glfw.h
	IMGUI/backends/imgui_impl_opengl3.h
	IMGUI/misc/cpp/imgui_stdlib.h
)
set(IMGUI_SOURCES
    IMGUI/imgui.cpp
    IMGUI/backends/imgui_impl_glfw.cpp
    IMGUI/backends/imgui_impl_opengl3.cpp
	IMGUI/imgui_demo.cpp
	IMGUI/imgui_tables.cpp
	IMGUI/imgui_widgets.cpp
	IMGUI/imgui_draw.cpp
	IMGUI/misc/cpp/imgui_stdlib.cpp
) 
add_library(ImGui
   ${IMGUI_SOURCES}
   ${IMGUI_INCLUDES}
)
target_include_directories(ImGui
    PUBLIC IMGUI
    PUBLIC IMGUI/backends
)
target_link_libraries(ImGui PUBLIC glad glfw)

add_library(Dependencies STBImplementation.cpp)
target_include_directories(Dependencies
    PUBLIC GLFW/include
    PUBLIC GLAD/include
    PUBLIC GLM
    PUBLIC ASSIMP/include
    PUBLIC STB_IMAGE
    PUBLIC YAML/include
	PUBLIC NFD/src/include
    PUBLIC .
)
target_link_libraries(Dependencies
    PUBLIC ImGui
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
    PUBLIC yaml-cpp
	PUBLIC nfd
)