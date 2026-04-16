cmake_minimum_required(VERSION 4.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(slv VERSION 1.0.0 LANGUAGES CXX)

set(slv_audio_channels 2 CACHE STRING "Number of audio channels")
set_property(CACHE slv_audio_channels PROPERTY STRINGS 1 2)
set(slv_audio_sample_rate 48000 CACHE STRING "Sound sample rate")

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(slv SHARED)
add_library(slv::slv ALIAS slv)

target_link_libraries(slv
    PUBLIC
        raylib
        glfw
        fmt::fmt
        tinyxml2::tinyxml2
        nlohmann_json::nlohmann_json
)

set(slv_engine_src
    src/engine/*.cpp
    src/engine/game/*.cpp
    src/engine/game/managers/*.cpp
    src/engine/game/objects/*.cpp
    src/engine/game/objects/abstract/*.cpp
)

set(slv_types_src
    src/types/*.cpp
)

file(GLOB_RECURSE engine_src CONFIGURE_DEPENDS ${slv_engine_src})
file(GLOB_RECURSE types_src CONFIGURE_DEPENDS ${slv_types_src})

target_sources(slv
    PRIVATE
        src/miniaudio.cpp
        src/internal/rl.cpp
        ${engine_src}
        ${types_src}
)

if(WIN32)
    target_sources(slv
        PRIVATE
            src/internal/platforms/windows.cpp
    )
endif()

target_include_directories(slv
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
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

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/slvConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/slvConfig.cmake
    INSTALL_DESTINATION lib/cmake/slv
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/slvConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS slv
    EXPORT slvTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT slvTargets
    NAMESPACE slv::
    DESTINATION lib/cmake/slv
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/slvConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/slvConfigVersion.cmake
    DESTINATION lib/cmake/slv
)