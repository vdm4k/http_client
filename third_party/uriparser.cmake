cmake_minimum_required(VERSION 3.14.0)

include(FetchContent)
FetchContent_Declare(
  uriparser
  GIT_REPOSITORY https://github.com/uriparser/uriparser
)

set(URIPARSER_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(URIPARSER_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(uriparser)
add_library(uriparser::uriparser ALIAS uriparser)
