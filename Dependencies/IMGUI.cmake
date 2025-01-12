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
add_library(ImGui ${IMGUI_SOURCES})
target_include_directories(ImGui
    PUBLIC IMGUI
    PUBLIC IMGUI/backends
    PUBLIC IMGUI/misc/cpp
)
target_link_libraries(ImGui PUBLIC glad glfw)

add_external_library(IMGUI IMGUI ImGui)