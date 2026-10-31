add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
)

if(APPLE)
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
  )
endif()

if(WIN32)
  if(MSVC)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>")
  else()
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:-mwindows>" "-static-libgcc" "-static-libstdc++" "-static")
  endif()
endif()


add_subdirectory(core)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}::core)
