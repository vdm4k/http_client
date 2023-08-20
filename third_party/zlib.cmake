cmake_minimum_required(VERSION 3.14.0)

find_package(ZLIB QUIET)
if (ZLIB_FOUND)
    return()
endif()

message(STATUS "couldn't find zlib in system. will download it")

include(FetchContent)
FetchContent_Declare(
  zlib
  GIT_REPOSITORY https://github.com/madler/zlib
  GIT_TAG v1.2.13
)

get_property(
    compile_options
    DIRECTORY
    PROPERTY COMPILE_OPTIONS
)

set_property(
    DIRECTORY
    APPEND
    PROPERTY COMPILE_OPTIONS -Wno-deprecated-non-prototype
)

FetchContent_GetProperties(zlib)
if(NOT zlib_POPULATED)
  FetchContent_Populate(zlib)
  add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set_property(
    DIRECTORY
    PROPERTY COMPILE_OPTIONS ${compile_options}
)

unset(compile_options)

target_include_directories(zlibstatic INTERFACE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
add_library(ZLIB::ZLIB ALIAS zlibstatic)
