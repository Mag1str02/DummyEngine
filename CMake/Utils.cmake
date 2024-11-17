function(configure Directory)
    message("---Configuring-${Directory}----------------------------------------------------------------------------")
    add_subdirectory(${Directory})
    message("---Configured-${Directory}-----------------------------------------------------------------------------")
endfunction(configure)

