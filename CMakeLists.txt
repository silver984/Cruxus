cmake_minimum_required(VERSION 3.21)

project(slv VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SLV_SOURCES CONFIGURE_DEPENDS "src/slv/*.cpp")
file(GLOB MINIAUDIO_SOURCES CONFIGURE_DEPENDS "src/miniaudio/*.cpp")
set(PLATFORM_SOURCES "")
if (WIN32)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "src/platforms/windows.cpp")
endif()

add_library(${PROJECT_NAME}
    SHARED
        ${SLV_SOURCES}
        ${MINIAUDIO_SOURCES}
        ${PLATFORM_SOURCES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SLV_EXPORTS
    PUBLIC
        SLV_AUDIO_CHANNELS=2
        SLV_AUDIO_SAMPLE_RATE=48000
        $<$<CONFIG:Debug>:SLV_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:SLV_RELWITHDEBINFO>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

include(FetchContent)

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

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
    PUBLIC
        fmt::fmt
        tinyxml2::tinyxml2
)
