cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Examples)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/../Sandbox)

add_subdirectory(src/DummyEngine)
add_subdirectory(src/DummyEditor)

add_executable(BasicScene src/Examples/BasicScene.cpp)
add_executable(EmptyApplication src/Examples/EmptyApplication.cpp)
add_executable(ImguiTest src/Examples/ImGuiTest.cpp)
add_executable(LoadingScene src/Examples/LoadingScene.cpp)
add_executable(HelloTriangle src/Examples/HelloTriangle.cpp)

target_link_libraries(BasicScene PUBLIC DummyEngineLib)
target_link_libraries(EmptyApplication PUBLIC DummyEngineLib)
target_link_libraries(ImguiTest PUBLIC DummyEngineLib)
target_link_libraries(LoadingScene PUBLIC DummyEngineLib)
target_link_libraries(HelloTriangle PUBLIC DummyEngineLib)
