message(STATUS "Fetching ImPlot...")

FetchContent_Declare(
        implot_fetch
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_TAG 419a8a0f5fcb77e1e7c19ab540441686bfe21bca
)
FetchContent_MakeAvailable(implot_fetch)

set(IMPLOT_SOURCES
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src/implot.cpp
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src/implot_demo.cpp
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src/implot_items.cpp
)

set(IMPLOT_HEADERS
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src/implot.h
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src/implot_internal.h
)

add_library(implot STATIC ${IMPLOT_SOURCES} ${IMPLOT_HEADERS})
target_include_directories(implot PUBLIC
        ${FETCHCONTENT_BASE_DIR}/implot_fetch-src
        ImGui-SFML::ImGui-SFML
)
target_link_libraries(implot PUBLIC ImGui-SFML::ImGui-SFML)

message(STATUS "ImPlot Fetched!")