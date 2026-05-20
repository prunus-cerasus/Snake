add_library(project_deps INTERFACE)

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.8
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3"
)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.0
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3_ttf"
)

FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(SDL3_ttf)

target_link_libraries(project_deps INTERFACE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)