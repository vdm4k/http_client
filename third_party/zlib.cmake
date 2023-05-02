cmake_minimum_required(VERSION 3.14.0)

find_package(ZLIB QUIET)
if (ZLIB_FOUND)
    return()
endif()

message(STATUS "couldn't find zlib in system. will download it")

include(FetchContent)
FetchContent_Declare(
  ZLIB
  GIT_REPOSITORY https://github.com/madler/zlib
  GIT_TAG v1.2.13
)

FetchContent_MakeAvailable(ZLIB)
add_library(ZLIB::ZLIB ALIAS zlib)
 
