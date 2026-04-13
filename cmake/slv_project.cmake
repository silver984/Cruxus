function(slv_setup_project project)
    target_link_libraries(${project}
        PRIVATE
            slv
    )

    if (WIN32)
        target_link_options(${project}
            PRIVATE
                $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
        )
    endif()
endfunction()

function(slv_project_resources project project_root resource_path)
    add_custom_command(TARGET ${project} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${project}>/${resource_path}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${project_root}/${resource_path} $<TARGET_FILE_DIR:${project}>/${resource_path}
    )
endfunction()