cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Editor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/../Sandbox)

set(EDITOR_SOURCES 
    Editor.cpp
    EditorLayer.cpp
    Panels/ViewportPanel.cpp
    Panels/ProfilerPanel.cpp
    Panels/SceneHierarchyPanel.cpp
    Panels/InspectorPanel.cpp
)

add_executable(DummyEditor 
    ${EDITOR_SOURCES}    
)
add_library(DummyEditorScripts SHARED 
    Scripts/EditorCameraController.cpp
)
add_dependencies(DummyEditor DummyEditorScripts)

target_link_libraries(DummyEditorScripts PUBLIC DummyEngineLib)
target_include_directories(DummyEditorScripts PUBLIC 
    ../.
)
target_link_libraries(DummyEditor PUBLIC DummyEngineLib)
target_include_directories(DummyEditor PUBLIC 
    .
    ../DummyEngine
)


