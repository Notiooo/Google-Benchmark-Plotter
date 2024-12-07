message(STATUS "Fetching ImGui-SFML...")

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG 98ab51bebc6d6f28c336347ccf1a632b41578a22
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui-sfml)


message(STATUS "ImGui-SFML Fetched!")