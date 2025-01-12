if (EXISTS ${CMAKE_SOURCE_DIR}/Config.cmake)
    include(Config.cmake)
endif()

include(CMake/Options.cmake)
include(CMake/Platform.cmake)
include(CMake/Utils.cmake)
include(CMake/Compiler.cmake)