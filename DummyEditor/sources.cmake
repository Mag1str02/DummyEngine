set(EDITOR_SOURCES 
    Editor.cpp
    EditorLayer.cpp

    ImGuiUtils/ImGuiPanel.cpp
    ImGuiUtils/ImGuiManager.cpp
    ImGuiUtils/ImGuiUtils.cpp

    Panels/ViewportPanel.cpp
    Panels/MenuBar.cpp
    Panels/ProfilerPanel.cpp
    Panels/SceneHierarchyPanel.cpp
    Panels/InspectorPanel.cpp
    Panels/ThemePanel.cpp

    Scripting/ScriptManager.cpp
)
if(${CMAKE_PLATFORM} STREQUAL "WINDOWS")
set(PLATFORM_SOURCES
    Platform/Windows/Scripting/Compiler.cpp
)
endif()
if(${CMAKE_PLATFORM} STREQUAL "LINUX")
set(PLATFORM_SOURCES
)
endif()
if(${CMAKE_PLATFORM} STREQUAL "APPLE")
set(PLATFORM_SOURCES
    Platform/Macos/Scripting/Compiler.cpp
)
endif()