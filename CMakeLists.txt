cmake_minimum_required(VERSION 3.21)

set(slv slv)
project(${slv} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${slv} SHARED)

file(GLOB_RECURSE slv_src CONFIGURE_DEPENDS src/slv/*.cpp)
file(GLOB miniaudio_src CONFIGURE_DEPENDS src/miniaudio/miniaudio.cpp)

set(compound_src
    ${slv_src}
    ${miniaudio_src}
)

set(platform)

if (WIN32)
    set(platform windows)
endif() # other platforms not supported yet

if (platform)
    file(GLOB_RECURSE platform_src CONFIGURE_DEPENDS
        src/platforms/${platform}.cpp
    )

    list(APPEND compound_src ${platform_src})
endif()

target_sources(${slv}
    PRIVATE
        ${compound_src}
)

target_include_directories(${slv}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${slv}
    PRIVATE
        SLV_EXPORTS
    PUBLIC
        SLV_AUDIO_CHANNELS=2
        SLV_AUDIO_SAMPLE_RATE=48000
        $<$<CONFIG:Debug>:SLV_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:SLV_RELWITHDEBINFO>
)

include(cmake/dependencies.cmake)
slv_fetch_dependencies(${slv})

include(cmake/slv_project.cmake)