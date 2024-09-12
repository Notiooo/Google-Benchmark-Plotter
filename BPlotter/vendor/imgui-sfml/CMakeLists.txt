message(STATUS "Fetching ImGui-SFML...")

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG 2a4dc2d33a4891148bb1ab150cfcfd0cb33c2b8c
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui-sfml)

message(STATUS "ImGui-SFML Fetched!")