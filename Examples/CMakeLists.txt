
add_executable(TestASAN TestASAN.cpp)
add_executable(TestTSAN TestTSAN.cpp)
add_executable(TestMSAN TestMSAN.cpp)
add_executable(TestLSAN TestLSAN.cpp)
add_executable(ProperGLFWUsage ProperGLFWUsage.cpp)
add_executable(TestOpenGLWithFiber TestOpenGLWithFiber.cpp)
add_executable(TracyFiber TracyFiber.cpp)

target_link_libraries(ProperGLFWUsage PUBLIC DummyEngineLib)
target_link_libraries(TestTSAN PUBLIC DummyEngineLib)
target_link_libraries(TestOpenGLWithFiber PUBLIC DummyEngineLib)
target_link_libraries(TracyFiber PUBLIC DummyEngineLib)


