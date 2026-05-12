add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
)

add_subdirectory(core)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::core)