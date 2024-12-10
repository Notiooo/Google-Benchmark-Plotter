include(FetchContent)

include(sfml/CMakeLists.txt)
include(imgui/CMakeLists.txt)
include(imgui-sfml/CMakeLists.txt)
include(implot/CMakeLists.txt)

add_subdirectory(spdlog)
add_subdirectory(result)
add_subdirectory(nlohmann_json)
add_subdirectory(ImGuiFileDialog)
add_subdirectory(cpp-httplib)
add_subdirectory(openssl-cmake)
add_subdirectory(semver)
