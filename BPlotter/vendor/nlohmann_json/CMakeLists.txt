message(STATUS "Fetching Nlohmann json...")

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG a006a7a48bb30a247f0344b788c62c2806edd90b
)

FetchContent_MakeAvailable(nlohmann_json)

message(STATUS "Nlohmann json Fetched!")