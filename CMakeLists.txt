cmake_minimum_required(VERSION 4.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(slv VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE slv_src CONFIGURE_DEPENDS src/slv/*.cpp)
file(GLOB miniaudio_src CONFIGURE_DEPENDS src/miniaudio/miniaudio.cpp)

set(slv_platform)
set(slv_compound_src ${slv_src} ${miniaudio_src})
set(slv_audio_channels 2 CACHE STRING "Number of audio channels")
set_property(CACHE slv_audio_channels PROPERTY STRINGS 1 2)
set(slv_audio_sample_rate 48000 CACHE STRING "Sound sample rate")

if (WIN32)
    set(slv_platform windows)
endif() # other platforms not supported yet

if (slv_platform)
    file(GLOB_RECURSE slv_platform_src CONFIGURE_DEPENDS
        src/platforms/${slv_platform}.cpp
    )

    list(APPEND slv_compound_src ${slv_platform_src})
endif()

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

add_library(slv SHARED)

target_link_libraries(slv
    PRIVATE
        raylib
        glfw
    PUBLIC
        fmt::fmt
        tinyxml2::tinyxml2
)

target_sources(slv
    PRIVATE
        ${slv_compound_src}
)

target_include_directories(slv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(slv
    PRIVATE
        SLV_EXPORTS
    PUBLIC
        SLV_AUDIO_CHANNELS=${slv_audio_channels}
        SLV_AUDIO_SAMPLE_RATE=${slv_audio_sample_rate}
        $<$<CONFIG:Debug>:SLV_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:SLV_RELWITHDEBINFO>
)

# expose functions from `slv_project.cmake` to projects that
# add SLV as a subdir
include(cmake/slv_project.cmake)