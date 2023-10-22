cmake_minimum_required(VERSION 3.14.0)
include(FetchContent)

FetchContent_Declare(
  llhttp  
  URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.1.3.tar.gz"
)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(llhttp)
