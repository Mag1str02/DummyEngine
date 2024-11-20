macro(set_option option help value)
    option(${option} ${help} ${value})
    if(${option})
        message(STATUS "${option}: ON")
    else()
        message(STATUS "${option}: OFF")
    endif()
endmacro()

set_option(DE_ENABLE_CLANG_TIDY_ON_COMPILATION "Run clang tidy during compilation" OFF)
set_option(DE_FORCE_NO_PCH "Prevent PCH" OFF)

