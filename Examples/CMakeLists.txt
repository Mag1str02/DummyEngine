
add_executable(TestASAN TestASAN.cpp)
add_executable(TestTSAN TestTSAN.cpp)
add_executable(TestMSAN TestMSAN.cpp)
add_executable(TestLSAN TestLSAN.cpp)

add_executable(ProperGLFWUsage ProperGLFWUsage.cpp)
target_link_libraries(ProperGLFWUsage PUBLIC DummyEngineLib)
target_link_libraries(TestTSAN PUBLIC DummyEngineLib)


