# Cruxus

cmake_minimum_required(VERSION 4.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cruxus VERSION 1.0.0 LANGUAGES CXX)

# TODO: options
# set(crx_audio_channels 2 CACHE STRING "Number of audio channels")
# set_property(CACHE crx_audio_channels PROPERTY STRINGS 1 2)
# set(crx_audio_sample_rate 48000 CACHE STRING "Sound sample rate")

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(cruxus SHARED)
add_library(cruxus::cruxus ALIAS cruxus)

add_subdirectory(cruxus)

target_include_directories(cruxus
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cruxus/internal
)

target_compile_definitions(cruxus
    PRIVATE
        CRUXUS_EXPORTS
    PUBLIC
        CRUXUS_AUDIO_CHANNELS=2
        CRUXUS_AUDIO_SAMPLE_RATE=48000
        $<$<CONFIG:Debug>:CRUXUS_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:CRUXUS_RELWITHDEBINFO>
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