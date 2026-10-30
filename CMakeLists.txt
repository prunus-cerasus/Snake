cmake_minimum_required(VERSION 3.20.0)

project(Snake LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM "3.5")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(RuntimeRPATH)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS On)
math(EXPR SYSTEM_BITNESS "${CMAKE_SIZEOF_VOID_P} * 8")
set(BASE_OUT "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${SYSTEM_BITNESS}/$<CONFIG>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUT}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUT}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUT}/staticlib")

if (MSVC)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUT}")
endif()

add_subdirectory(lib)
add_subdirectory(src)

set(ASSET_DEST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${ASSET_DEST_DIR}/assets"
  COMMENT "Syncing game assets to build directory..."
)

