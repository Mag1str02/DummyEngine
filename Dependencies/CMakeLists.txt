include(ASSIMP.cmake)
include(GLAD.cmake)
include(GLFW.cmake)
include(GLM.cmake)
include(IMGUI.cmake)
include(NFD.cmake)
include(YAML.cmake)
include(STB_IMAGE.cmake)

function (link_dependencies TARGET)
    target_include_directories(${TARGET}
        PUBLIC ${EXTERNAL_ASSIMP_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_GLAD_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_GLFW_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_GLM_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_IMGUI_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_NFD_INCLUDE_DIR}
        PUBLIC ${EXTERNAL_YAML_INDCLUDE_DIR}
        PUBLIC ${EXTERNAL_STB_IMAGE_INCLUDE_DIR}
    )
    target_link_libraries(${TARGET}
        PUBLIC ${EXTERNAL_ASSIMP_LIBRARY}
        PUBLIC ${EXTERNAL_GLAD_LIBRARY}
        PUBLIC ${EXTERNAL_GLFW_LIBRARY}
        PUBLIC ${EXTERNAL_GLM_LIBRARY}
        PUBLIC ${EXTERNAL_IMGUI_LIBRARY}
        PUBLIC ${EXTERNAL_NFD_LIBRARY}
        PUBLIC ${EXTERNAL_YAML_LIBRARY}
        PUBLIC ${EXTERNAL_STB_IMAGE_LIBRARY}
    )
endfunction(link_dependencies)

add_custom_target(BuildDependencies)
add_dependencies(BuildDependencies 
    ${EXTERNAL_ASSIMP_LIBRARY}
    ${EXTERNAL_GLAD_LIBRARY}
    ${EXTERNAL_GLFW_LIBRARY}
    ${EXTERNAL_GLM_LIBRARY}
    ${EXTERNAL_IMGUI_LIBRARY}
    ${EXTERNAL_NFD_LIBRARY}
    ${EXTERNAL_YAML_LIBRARY}
    ${EXTERNAL_STB_IMAGE_LIBRARY}
)