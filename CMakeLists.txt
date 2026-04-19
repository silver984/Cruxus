# Cruxus

cmake_minimum_required(VERSION 4.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cruxus VERSION 1.0.0 LANGUAGES CXX)

set(crx_audio_channels 2 CACHE STRING "Number of audio channels")
set_property(CACHE crx_audio_channels PROPERTY STRINGS 1 2)
set(crx_audio_sample_rate 48000 CACHE STRING "Sound sample rate")

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(cruxus SHARED)
add_library(cruxus::cruxus ALIAS cruxus)

target_sources(cruxus
    PRIVATE
        crx/engine/debug/log.cc
        crx/engine/math/mat3.cc
)

add_subdirectory(crx/external)
add_subdirectory(crx/internal)
add_subdirectory(crx/engine/main)

target_include_directories(cruxus
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/crx/internal
)

target_compile_definitions(cruxus
    PRIVATE
        CRX_EXPORTS
    PUBLIC
        CRX_AUDIO_CHANNELS=${crx_audio_channels}
        CRX_AUDIO_SAMPLE_RATE=${crx_audio_sample_rate}
        $<$<CONFIG:Debug>:CRX_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:CRX_RELWITHDEBINFO>
)

target_link_libraries(cruxus
    PUBLIC
        raylib
        glfw
        fmt::fmt
        tinyxml2::tinyxml2
        nlohmann_json::nlohmann_json
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cruxusConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cruxusConfig.cmake
    INSTALL_DESTINATION lib/cmake/cruxus
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cruxusConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS cruxus
    EXPORT cruxusTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT cruxusTargets
    NAMESPACE cruxus::
    DESTINATION lib/cmake/cruxus
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cruxusConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cruxusConfigVersion.cmake
    DESTINATION lib/cmake/cruxus
)