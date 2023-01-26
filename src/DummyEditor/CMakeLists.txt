cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(sources.cmake)

add_executable(DummyEditor 
    ${EDITOR_SOURCES}    
    ${PLATFORM_SOURCES}
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
    ../.
)


