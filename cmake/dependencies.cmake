include(FetchContent)

function(slv_fetch_dependencies slv)
    # raylib
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(raylib)

    # fmt
    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 12.0.0
    )
    
    FetchContent_MakeAvailable(fmt)

    # tinyxml2
    set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(tinyxml2
        GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
        GIT_TAG 11.0.0
    )
    
    FetchContent_MakeAvailable(tinyxml2)

    target_link_libraries(${slv}
        PRIVATE raylib
        PUBLIC fmt::fmt tinyxml2::tinyxml2
    )
endfunction()