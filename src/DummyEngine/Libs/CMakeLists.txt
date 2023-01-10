cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(assimp)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(yaml)



set(IMGUI_INCLUDES
	imgui/imgui.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_opengl3.h
	imgui/misc/cpp/imgui_stdlib.h
)
set(IMGUI_SOURCES
	imgui/imgui.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_draw.cpp
	imgui/misc/cpp/imgui_stdlib.cpp
)
add_library(imgui
   ${IMGUI_SOURCES}
   ${IMGUI_INCLUDES}
)
target_include_directories(imgui
    PUBLIC imgui
    PUBLIC imgui/backends
)
target_link_libraries(imgui PUBLIC glad glfw)



add_library(Dependencies STBImplementation.cpp)
target_include_directories(Dependencies
    PUBLIC glfw/include
    PUBLIC glad/include
    PUBLIC glm
    PUBLIC assimp/include
    PUBLIC stb_image   
    PUBLIC yaml/include
    PUBLIC .
)
target_link_libraries(Dependencies
    PUBLIC imgui
    PUBLIC glad
    PUBLIC glfw
    PUBLIC assimp
    PUBLIC yaml-cpp
)