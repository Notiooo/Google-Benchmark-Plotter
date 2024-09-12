message(STATUS "Fetching ImGui...")

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG c6aa051629753f0ef0d26bf775a8b6a92aa213b2 # release v1.90.4 DOCKING
)
FetchContent_MakeAvailable(imgui)

message(STATUS "ImGui Fetched!")