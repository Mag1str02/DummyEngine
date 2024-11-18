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
target_include_directories(DummyEngineLib PUBLIC ../.)

if (${ENABLE_PRECOMPILED_HEADERS})
    target_precompile_headers(DummyEngineLib PUBLIC ${ENGINE_PRECOMPILED_HEADERS})
endif()