message(STATUS "Fetching ImGuiFileDialog...")

FetchContent_Declare(
        ImGuiFileDialog
        GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git
        GIT_TAG c66089f509bbfbc0d8852d54106b1af88a104822
)

FetchContent_MakeAvailable(ImGuiFileDialog)

target_link_libraries(ImGuiFileDialog PUBLIC ImGui-SFML::ImGui-SFML)

message(STATUS "ImGuiFileDialog Fetched!")