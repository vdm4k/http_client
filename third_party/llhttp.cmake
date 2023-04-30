cmake_minimum_required(VERSION 3.14.0)
include(FetchContent)

FetchContent_Declare(
  llhttp
  URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v8.1.0.tar.gz"
)

FetchContent_MakeAvailable(llhttp)
