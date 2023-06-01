set(SANDBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Sandbox)
set(EXTERNAL_LIBS_DIR libs)
set(DE_LIB_SOURCE_DIR .) 

include(sources.cmake)

add_subdirectory(Libs)
add_library(DummyEngineLib SHARED
   ${ENGINE_SOURCES}
   ${PLATFORM_SOURCES}        Core/SoundEngine/AudioEngineFacade.h Core/SoundEngine/AudioEngineFacade.cpp Core/SoundEngine/AudioFile.cpp Core/SoundEngine/AudioFile.h Core/SoundEngine/AudioContext.cpp Core/SoundEngine/AudioContext.h Core/SoundEngine/AudioEngine.cpp Core/SoundEngine/AudioEngine.h Core/SoundEngine/SoundsAndFactories.cpp Core/SoundEngine/SoundsAndFactories.h Core/SoundEngine/WAVLoader.h Core/SoundEngine/WAVLoader.cpp)

target_compile_definitions(DummyEngineLib PUBLIC 
    DE_ENABLE_PROFILER=1
    DE_ENABLE_ASSERTS=1
    DE_ENABLE_LOGGING=1
)

get_target_property(DependenciesInclude Dependencies INCLUDE_DIRECTORIES)

target_include_directories(DummyEngineLib
    PUBLIC ../.
)
target_link_libraries(DummyEngineLib
    PUBLIC Dependencies    
)