cmake_minimum_required(VERSION 3.14.0)
include(FetchContent)

FetchContent_Declare(
  llhttp
  GIT_REPOSITORY https://github.com/vdm4k/llhttp-release-v8.1.0.git
)

FetchContent_MakeAvailable(llhttp)
