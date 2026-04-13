function(slv_collect_platform_sources OUT_VAR)
    set(SRCS)

    if (WIN32)
        file(GLOB_RECURSE WIN_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/windows.cpp)
        list(APPEND SRCS ${WIN_SRC})
    endif()

    set(${OUT_VAR} ${SRCS} PARENT_SCOPE)
endfunction()