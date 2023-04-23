cmake_minimum_required(VERSION 3.14.0)
include(FetchContent)
FetchContent_Declare(
  dns_resolver
  GIT_REPOSITORY https://github.com/vdm4k/dns_resolver
)

FetchContent_MakeAvailable(dns_resolver)
