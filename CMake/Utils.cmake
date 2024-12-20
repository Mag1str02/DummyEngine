macro(add_external_library DIRECTORY INCLUDE_DIR LIBRARY)
    message(STATUS "---Configuring-${LIBRARY}----------------------------------------------------------------------------")
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${DIRECTORY}/CMakeLists.txt)
        message(STATUS "Adding ${DIRECTORY} as a subdirectory")
        add_subdirectory(${DIRECTORY})
    else() 
        message(STATUS "Adding ${DIRECTORY} as managed library")
    endif()
    set(EXTERNAL_${DIRECTORY}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/${INCLUDE_DIR} PARENT_SCOPE)
    set(EXTERNAL_${DIRECTORY}_LIBRARY ${LIBRARY} PARENT_SCOPE)
    set(EXTERNAL_${DIRECTORY}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/${INCLUDE_DIR})
    set(EXTERNAL_${DIRECTORY}_LIBRARY ${LIBRARY})
    message(STATUS "${DIRECTORY} include dir: ${INCLUDE_DIR}")
    message(STATUS "${DIRECTORY} library: ${LIBRARY}")
    message(STATUS "---Configured-${LIBRARY}-----------------------------------------------------------------------------")
endmacro(add_external_library)

macro(save_compile_flags)
    set(CXX_BUFF "${CMAKE_CXX_FLAGS}")
    set(C_BUFF "${CMAKE_C_FLAGS}")
endmacro(save_compile_flags)

macro(restore_compile_flags)
    set(CMAKE_CXX_FLAGS "${CXX_BUFF}")
    set(CMAKE_C_FLAGS "${C_BUFF}")
endmacro(restore_compile_flags)

add_custom_target(FormatDummyEngine
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/Format.py
    COMMENT "Running clang-format for DummyEngine"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/DummyEngine
)
add_custom_target(FormatDummyEditor
    COMMAND python ${CMAKE_CURRENT_LIST_DIR}/Format.py
    COMMENT "Running clang-format for DummyEditor"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/DummyEditor
)
add_custom_target(FormatAll)
add_dependencies(FormatAll FormatDummyEditor FormatDummyEngine)

