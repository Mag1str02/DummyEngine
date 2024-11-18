set(YAML_CPP_BUILD_CONTRIB OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
set(YAML_CPP_BUILD_TESTS OFF)

save_compile_flags()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-#pragma-messages")
add_external_library(YAML YAML/include yaml-cpp)
restore_compile_flags()