set(core_SOURCES
    SDLState.cpp
    Timer.cpp
    Game.cpp
    state/Menu.cpp
    state/Gameplay.cpp
)

add_library(${PROJECT_NAME}_core STATIC ${core_SOURCES})
target_include_directories(${PROJECT_NAME}_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}_core PUBLIC project_deps)

set_target_properties(${PROJECT_NAME}_core PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
)

if(MSVC)
    target_compile_options(${PROJECT_NAME}_core PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME}_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(${PROJECT_NAME}::core ALIAS ${PROJECT_NAME}_core)