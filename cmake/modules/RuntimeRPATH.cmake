if(UNIX)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

  if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/lib")
  else()
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/lib")
  endif()

  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

