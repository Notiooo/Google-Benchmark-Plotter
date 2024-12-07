message(STATUS "Fetching SFML 3...")

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG f930cbc562a31ebdb09aaf2c37d77a33f8de987d # SFML 3
)

# No need to build network modules
set(SFML_BUILD_NETWORK FALSE)
set(SFML_BUILD_AUDIO FALSE)

FetchContent_MakeAvailable(SFML)

message(STATUS "SFML 3 Fetched!")