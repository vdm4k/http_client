cmake_minimum_required(VERSION 3.14.0)

include(FetchContent)
FetchContent_Declare(
  uriparser
  GIT_REPOSITORY https://github.com/uriparser/uriparser
)
FetchContent_MakeAvailable(uriparser)
add_library(uriparser::uriparser ALIAS uriparser)
