message(STATUS "Fetching ImGui...")

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG c71e4e8c7cb9b42b460bbaedfa4bc443f885b05b # v1.91.4-docking
)
FetchContent_MakeAvailable(imgui)

message(STATUS "ImGui Fetched!")