cmake_minimum_required(VERSION 3.24)

project(BPlotterSrc VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CMakeLists_Sources.txt)

add_library(BPlotterSrc STATIC ${PROJECT_SOURCES})
target_precompile_headers(BPlotterSrc PUBLIC pch.hpp)

set(CUSTOM_INCLUDES_DIR ${CMAKE_CURRENT_BINARY_DIR}/custom_includes)
file(MAKE_DIRECTORY ${CUSTOM_INCLUDES_DIR})

execute_process(
        COMMAND git describe --tags
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DGIT_TAG="${GIT_TAG}")


set(CUSTOM_INCLUDES
        
)

foreach (HEADER ${CUSTOM_INCLUDES})
    configure_file(
            ${HEADER}
            ${CUSTOM_INCLUDES_DIR}
            COPYONLY
    )
endforeach ()

target_include_directories(BPlotterSrc PUBLIC
        ${FETCHCONTENT_BASE_DIR}/result-src/include
        ${CUSTOM_INCLUDES_DIR}
)

set(NEEDED_HTTPLIB_LIBRARIES
        httplib
        ssl
        crypto
        ws2_32
        crypt32
        mingw32
        gcc
)

target_link_libraries(BPlotterSrc
        PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
        ImGui-SFML::ImGui-SFML
        spdlog
        implot
        nlohmann_json
        ImGuiFileDialog
        ${NEEDED_HTTPLIB_LIBRARIES}
        semver
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(BPlotterSrc PUBLIC SFML::Main)
endif()

# Remove previous engine resources

add_custom_command(TARGET BPlotterSrc PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:BPlotterSrc>/resources
)

# Copy application resources

add_custom_command(TARGET BPlotterSrc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:BPlotterSrc>/resources
)

# Copy imgui config
add_custom_command(
        TARGET BPlotterSrc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env SOURCE_FILE="${CMAKE_SOURCE_DIR}/imgui.ini"
        DEST_DIR="$<TARGET_FILE_DIR:BPlotterSrc>"
        ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/copy_file_if_not_exist.cmake
        COMMENT "Ensure imgui.ini exists in the target directory."
)

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(BPlotterApp main.cpp ${CMAKE_SOURCE_DIR}/resources.rc)
    else()
        add_executable(BPlotterApp WIN32 main.cpp ${CMAKE_SOURCE_DIR}/resources.rc)
    endif()
else()
    add_executable(BPlotterApp main.cpp)
endif()

target_link_libraries(BPlotterApp PRIVATE BPlotterSrc)
