cmake_minimum_required(VERSION 3.21)
project(slv VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(slv SHARED)

target_include_directories(slv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SLV_SOURCES CONFIGURE_DEPENDS src/slv/*.cpp)
file(GLOB MINIAUDIO_SOURCES CONFIGURE_DEPENDS src/miniaudio/*.cpp)

include(cmake/platform.cmake)
slv_collect_platform_sources(PLATFORM_SOURCES)

target_sources(slv PRIVATE
    ${SLV_SOURCES}
    ${MINIAUDIO_SOURCES}
    ${PLATFORM_SOURCES}
)

target_compile_definitions(slv
    PRIVATE SLV_EXPORTS
    PUBLIC
        SLV_AUDIO_CHANNELS=2
        SLV_AUDIO_SAMPLE_RATE=48000
        $<$<CONFIG:Debug>:SLV_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:SLV_RELWITHDEBINFO>
)

include(cmake/dependencies.cmake)
slv_fetch_dependencies()

target_link_libraries(slv
    PRIVATE raylib
    PUBLIC fmt::fmt tinyxml2::tinyxml2
)