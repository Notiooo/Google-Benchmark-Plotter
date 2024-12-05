cmake_minimum_required(VERSION 3.24)

project(BPlotterBenchmarks LANGUAGES CXX)

include_directories(../src)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
enable_testing()

configure_file("CMakeLists.txt.in" "${CMAKE_BINARY_DIR}/googlebenchmark-download/CMakeLists.txt")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googlebenchmark-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googlebenchmark-download"
)

#set(benchmark_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_BINARY_DIR}/googlebenchmark-src"
        "${CMAKE_BINARY_DIR}/googlebenchmark-build"
)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${benchmark_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include"
    )
endif ()

include(benchmarks/CMakeLists.txt)
include(../tests/mocks/CMakeLists.txt)
include_directories(benchmarks/src)
include_directories(../tests/mocks/src)
list(TRANSFORM Benchmark_Sources PREPEND benchmarks/)
list(TRANSFORM Mocks_Sources PREPEND ../tests/mocks/)

add_executable(BPlotterBenchmarks
        ${Benchmark_Sources}
        ${Mocks_Sources}
        main.cpp
        benchmarks/src/SampleFibonacciBenchmark.cpp
)

target_link_libraries(BPlotterBenchmarks PRIVATE BPlotterSrc)
target_link_libraries(BPlotterBenchmarks PRIVATE benchmark benchmark_main)
target_link_libraries(BPlotterBenchmarks PRIVATE gmock gmock_main)

# Remove previous application resources
add_custom_command(TARGET BPlotterBenchmarks PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:BPlotterBenchmarks>/resources
)

# Copy application resources
add_custom_command(TARGET BPlotterBenchmarks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:BPlotterBenchmarks>/resources
)