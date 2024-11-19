set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC -fno-omit-frame-pointer")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Sandbox)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Sandbox)

set(ENABLE_PRECOMPILED_HEADERS OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(ENABLE_PRECOMPILED_HEADERS ON)
endif()

function(add_warnings TARGET)
    target_compile_options(${TARGET} PUBLIC -Wall -Wextra -Wpedantic -Wno-extra-semi -Wno-missing-field-initializers)

    if (${ENABLE_PRECOMPILED_HEADERS})
        target_compile_options(${TARGET} PUBLIC -Winvalid-pch)
    endif()
endfunction(add_warnings)