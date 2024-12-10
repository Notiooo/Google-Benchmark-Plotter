message(STATUS "Fetching openssl-cmake...")

FetchContent_Declare(
        openssl-cmake
        GIT_REPOSITORY https://github.com/viaduck/openssl-cmake
        GIT_TAG 186966f4ae15925b0dd7f370cdbb654b00e134e2
)

set(OPENSSL_USE_STATIC_LIBS ON)

FetchContent_MakeAvailable(openssl-cmake)

message(STATUS "openssl-cmake Fetched!")