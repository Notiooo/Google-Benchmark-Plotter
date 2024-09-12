message(STATUS "Fetching Spdlog...")

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)
target_compile_definitions(spdlog PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
target_compile_definitions(spdlog PUBLIC _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)

message(STATUS "Spdlog Fetched!")