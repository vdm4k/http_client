cmake_minimum_required(VERSION 3.14.0)

find_package(fmt "9.1.0" QUIET)
if (fmt_FOUND)
    return()
endif()

message(STATUS "couldn't find libfmt in system. will download it")

include(FetchContent)
FetchContent_Declare(
  libfmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  TIMEOUT 10
  GIT_TAG 9.1.0
)

FetchContent_MakeAvailable(libfmt)
