cmake_minimum_required(VERSION 3.24)

project(BenchmarkPlotterSrc VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CMakeLists_Sources.txt)

add_library(BenchmarkPlotterSrc STATIC ${PROJECT_SOURCES})
target_precompile_headers(BenchmarkPlotterSrc PUBLIC pch.hpp)

set(CUSTOM_INCLUDES_DIR ${CMAKE_CURRENT_BINARY_DIR}/custom_includes)
file(MAKE_DIRECTORY ${CUSTOM_INCLUDES_DIR})

set(CUSTOM_INCLUDES
        
)

foreach (HEADER ${CUSTOM_INCLUDES})
    configure_file(
            ${HEADER}
            ${CUSTOM_INCLUDES_DIR}
            COPYONLY
    )
endforeach ()

target_include_directories(BenchmarkPlotterSrc PUBLIC
        ${FETCHCONTENT_BASE_DIR}/result-src/include
        ${CUSTOM_INCLUDES_DIR}
)

target_link_libraries(BenchmarkPlotterSrc
        PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
        ImGui-SFML::ImGui-SFML
        spdlog
)

# Remove previous engine resources

add_custom_command(TARGET BenchmarkPlotterSrc PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:BenchmarkPlotterSrc>/resources
)

# Copy application resources

add_custom_command(TARGET BenchmarkPlotterSrc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:BenchmarkPlotterSrc>/resources
)

# Copy imgui config
add_custom_command(
        TARGET BenchmarkPlotterSrc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env SOURCE_FILE="${CMAKE_SOURCE_DIR}/imgui.ini"
        DEST_DIR="$<TARGET_FILE_DIR:BenchmarkPlotterSrc>"
        ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/copy_file_if_not_exist.cmake
        COMMENT "Ensure imgui.ini exists in the target directory."
)


add_executable(BenchmarkPlotterApp main.cpp)
target_link_libraries(BenchmarkPlotterApp PRIVATE BenchmarkPlotterSrc)
