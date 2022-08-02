cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Examples)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/../Sandbox)

add_subdirectory(src/DummyEngine)

add_executable(BasicScene src/Examples/BasicScene.cpp)
add_executable(Empty src/Examples/Empty.cpp)
add_executable(UUID src/Examples/UUID.cpp)

target_link_libraries(BasicScene PUBLIC DummyEngineLib)
target_link_libraries(Empty PUBLIC DummyEngineLib)
target_link_libraries(UUID PUBLIC DummyEngineLib)
