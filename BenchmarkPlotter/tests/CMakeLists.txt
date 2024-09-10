cmake_minimum_required(VERSION 3.24)

project(BenchmarkPlotterTests LANGUAGES CXX)

include_directories(../src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
enable_testing()

configure_file("CMakeLists.txt.in" "${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
        "${CMAKE_BINARY_DIR}/googletest-build"
)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include"
    )
endif ()

include(ut/CMakeLists.txt)
include(mocks/CMakeLists.txt)
include_directories(ut/src)
include_directories(mocks/src)
list(TRANSFORM UT_Sources PREPEND ut/)
list(TRANSFORM Mocks_Sources PREPEND mocks/)

add_executable(BenchmarkPlotterTests
        main.cpp
        ${UT_Sources}
        ${Mocks_Sources}
)

target_link_libraries(BenchmarkPlotterTests PRIVATE BenchmarkPlotterSrc)
target_link_libraries(BenchmarkPlotterTests PRIVATE gtest gtest_main)
target_link_libraries(BenchmarkPlotterTests PRIVATE gmock gmock_main)


# Remove previous application resources

add_custom_command(TARGET BenchmarkPlotterTests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:BenchmarkPlotterTests>/resources
)

# Copy application resources

add_custom_command(TARGET BenchmarkPlotterTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:BenchmarkPlotterTests>/resources
)