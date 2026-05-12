add_library(project_deps INTERFACE)

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.8
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3"
)

set(SDL_WAYLAND On CACHE BOOL "Enable Wayland support" FORCE)
set(SDL_WAYLAND_LIBDECOR On CACHE BOOL "Enable Wayland client-side decorations" FORCE)

FetchContent_MakeAvailable(SDL3)

target_link_libraries(project_deps INTERFACE
    SDL3::SDL3
)