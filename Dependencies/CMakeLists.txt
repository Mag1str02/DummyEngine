include(ASSIMP.cmake)
include(GLAD.cmake)
include(GLFW.cmake)
include(GLM.cmake)
include(IMGUI.cmake)

add_subdirectory(YAML)
add_subdirectory(NFD)

add_library(Dependencies STBImplementation.cpp)
target_include_directories(Dependencies
    PUBLIC ${EXTERNAL_ASSIMP_INCLUDE_DIR}
    PUBLIC ${EXTERNAL_GLAD_INCLUDE_DIR}
    PUBLIC ${EXTERNAL_GLFW_INCLUDE_DIR}
    PUBLIC ${EXTERNAL_GLM_INCLUDE_DIR}
    PUBLIC ${EXTERNAL_IMGUI_INCLUDE_DIR}
    PUBLIC STB_IMAGE
    PUBLIC YAML/include
	PUBLIC NFD/src/include
)
target_link_libraries(Dependencies
    PUBLIC ${EXTERNAL_ASSIMP_LIBRARY}
    PUBLIC ${EXTERNAL_GLAD_LIBRARY}
    PUBLIC ${EXTERNAL_GLFW_LIBRARY}
    PUBLIC ${EXTERNAL_GLM_LIBRARY}
    PUBLIC ${EXTERNAL_IMGUI_LIBRARY}
    PUBLIC yaml-cpp
	PUBLIC nfd
)