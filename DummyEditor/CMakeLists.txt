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
        target_precompile_headers(DummyEditorScripts REUSE_FROM DummyEngineLib)
endif()
if (${DE_CLANG_TIDY_RUN_ON_COMPILATION})
    set_target_properties(DummyEditor PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()