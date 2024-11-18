include(Sources.cmake)

add_library(DummyEditorScripts SHARED Scripts/EditorCameraController.cpp)
target_link_libraries(DummyEditorScripts PUBLIC DummyEngineLib)
target_include_directories(DummyEditorScripts PUBLIC ../.)

add_executable(DummyEditor
        ${EDITOR_SOURCES}
        ${PLATFORM_SOURCES}
)
target_compile_definitions(DummyEditor PUBLIC DE_EDITOR_LIBRARY_NAME="libDummyEditorScripts")
target_link_libraries(DummyEditor PUBLIC DummyEngineLib)
target_include_directories(DummyEditor PUBLIC ../.)
add_dependencies(DummyEditor DummyEditorScripts)

if (${ENABLE_PRECOMPILED_HEADERS})
        target_precompile_headers(DummyEditor REUSE_FROM DummyEngineLib)
endif()