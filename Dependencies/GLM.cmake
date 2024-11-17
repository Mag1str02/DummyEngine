set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(GLM_BUILD_LIBRARY ON)
set(GLM_ENABLE_CXX_20 ON)

# TODO: check glm simd options

add_external_library(GLM GLM/include glm)
target_compile_definitions(glm PUBLIC GLM_ENABLE_EXPERIMENTAL)