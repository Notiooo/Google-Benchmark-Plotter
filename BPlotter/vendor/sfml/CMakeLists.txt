message(STATUS "Fetching SFML 3...")

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 016e201d179966fa4940d5d04d0f96bd72fce01c # SFML 3
)

# No need to build network modules
set(SFML_BUILD_NETWORK FALSE)
set(SFML_BUILD_AUDIO FALSE)

FetchContent_MakeAvailable(SFML)

message(STATUS "SFML 3 Fetched!")