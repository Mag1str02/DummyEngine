cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Examples)
set(SANDBOX_DIR ${CMAKE_BINARY_DIR}/../Sandbox)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${SANDBOX_DIR})

add_subdirectory(src/DummyEngine)

add_executable(New_Model src/Examples/new_model.cpp)

execute_process(COMMAND cls)

target_link_libraries(New_Model PUBLIC DummyEngineLib)
