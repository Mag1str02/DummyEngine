cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)

project(ImGui)
    

set(IMGUI_INCLUDES
	imgui/imgui.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	imgui/imgui.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_draw.cpp
)

add_library(ImGui
   ${IMGUI_SOURCES}
   ${IMGUI_INCLUDES}
)

target_include_directories(ImGui
    PUBLIC imgui
    PUBLIC imgui/backends
)
target_link_libraries(ImGui PUBLIC glad glfw)


