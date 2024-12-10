message(STATUS "Fetching cpp-httplib...")

FetchContent_Declare(
        cpp-httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
        GIT_TAG a7bc00e3307fecdb4d67545e93be7b88cfb1e186
)
FetchContent_MakeAvailable(cpp-httplib)

message(STATUS "cpp-httplib Fetched!")