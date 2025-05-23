set(SANDBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Sandbox)
set(EXTERNAL_LIBS_DIR libs)
set(DE_LIB_SOURCE_DIR .) 

include(Sources.cmake)

add_library(DummyEngineLib SHARED
   ${ENGINE_SOURCES}
   ${PLATFORM_SOURCES}
)
link_dependencies(DummyEngineLib)
target_compile_definitions(DummyEngineLib PUBLIC 
    DE_ENABLE_PROFILER=1
    DE_ENABLE_ASSERTS=1
    DE_ENABLE_LOGGING=1
)

if (DE_USE_GNU_FOR_SCRIPTS)
    target_compile_definitions(DummyEngineLib PUBLIC DE_USE_GNU_FOR_SCRIPTS=1)
endif()

target_include_directories(DummyEngineLib PUBLIC ../.)
add_warnings(DummyEngineLib)

if (ENABLE_PRECOMPILED_HEADERS)
    target_precompile_headers(DummyEngineLib PUBLIC ${ENGINE_PRECOMPILED_HEADERS})
endif()
if (DE_CLANG_TIDY_RUN_ON_COMPILATION)
    set_target_properties(DummyEngineLib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()
if (DE_PLATFORM STREQUAL "WINDOWS")
    target_compile_definitions(DummyEngineLib PUBLIC YAML_CPP_STATIC_DEFINE)
endif()
if (DE_FORMAT_BEFORE_BUILD)
    add_dependencies(DummyEngineLib FormatAll)
endif()