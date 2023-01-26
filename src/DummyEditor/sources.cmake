set(EDITOR_SOURCES 
    Editor.cpp
    EditorLayer.cpp
    Panels/ViewportPanel.cpp
    Panels/ProfilerPanel.cpp
    Panels/SceneHierarchyPanel.cpp
    Panels/InspectorPanel.cpp
)
if(${CMAKE_PLATFORM} STREQUAL "WINDOWS")
set(PLATFORM_SOURCES
)
endif()
if(${CMAKE_PLATFORM} STREQUAL "LINUX")
set(PLATFORM_SOURCES
)
endif()
if(${CMAKE_PLATFORM} STREQUAL "MACOS")
set(PLATFORM_SOURCES
)
endif()