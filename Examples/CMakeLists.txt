add_executable(EmptyApplication EmptyApplication.cpp)
add_executable(ImguiTest ImGuiTest.cpp)
add_executable(HelloTriangle HelloTriangle.cpp)
add_executable(Test Test.cpp)

target_link_libraries(EmptyApplication PUBLIC DummyEngineLib)
target_link_libraries(ImguiTest PUBLIC DummyEngineLib)
target_link_libraries(HelloTriangle PUBLIC DummyEngineLib)
target_link_libraries(Test PUBLIC DummyEngineLib)
