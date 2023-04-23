cmake_minimum_required(VERSION 3.14.0)
include(FetchContent)
FetchContent_Declare(
  network
  GIT_REPOSITORY https://github.com/vdm4k/network
)

FetchContent_MakeAvailable(network)
