cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ASSIMP)
add_subdirectory(GLFW)
add_subdirectory(GLAD)
add_subdirectory(YAML)



set(IMGUI_INCLUDES
	ImGui/imgui.h
	ImGui/backends/imgui_impl_glfw.h
	ImGui/backends/imgui_impl_opengl3.h
	ImGui/misc/cpp/imgui_stdlib.h
)
set(IMGUI_SOURCES
	ImGui/imgui.cpp
	ImGui/backends/imgui_impl_glfw.cpp
	ImGui/backends/imgui_impl_opengl3.cpp
	ImGui/imgui_demo.cpp
	ImGui/imgui_tables.cpp
	ImGui/imgui_widgets.cpp
	ImGui/imgui_draw.cpp
	ImGui/misc/cpp/imgui_stdlib.cpp
)
add_library(imgui
   ${IMGUI_SOURCES}
   ${IMGUI_INCLUDES}
)
target_include_directories(imgui
    PUBLIC ImGui
    PUBLIC ImGui/backends
)
target_link_libraries(imgui PUBLIC glad glfw)



add_library(Dependencies STBImplementation.cpp)
target_include_directories(Dependencies
    PUBLIC GLFW/include
    PUBLIC GLAD/include
    PUBLIC GLM
    PUBLIC ASSIMP/include
    PUBLIC STB_IMAGE
    PUBLIC YAML/include
    PUBLIC .
)
target_link_libraries(Dependencies
    PUBLIC imgui
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
    PUBLIC yaml-cpp
    PUBLIC dbghelp
)