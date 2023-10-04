cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -g")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")

project(DummyEngine)

include(platform.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Sandbox)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Sandbox)

add_subdirectory(DummyEngine)
add_subdirectory(DummyEditor)
add_subdirectory(Examples)