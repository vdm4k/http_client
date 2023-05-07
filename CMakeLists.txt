cmake_minimum_required(VERSION 3.14.0)
project(http_client VERSION 0.9.0 DESCRIPTION "http client library")

# core h files
set(H_FILES
    include/http_client/request.h
    include/http_client/status.h
    include/http_client/header.h
    include/http_client/response.h
    include/http_client/zlib_helper.h
)

# core cpp files
set(CPP_FILES
    source/request.cpp
    source/header.cpp
    source/status.cpp
    source/response.cpp
    source/zlib_helper.cpp
)

SET(WITH_TCP_SSL ON CACHE BOOL "" FORCE)

include("${CMAKE_SOURCE_DIR}/third_party/llhttp.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/network.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/cli11.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/libfmt.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/uriparser.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/dns_resolver.cmake")
include("${CMAKE_SOURCE_DIR}/third_party/zlib.cmake")
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}  ${H_FILES} ${CPP_FILES}  main.cpp )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE  -Wall -Wextra -Wpedantic -Wno-vla -Wno-unused-local-typedefs)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

#sanitizer
option(WITH_SANITIZER "Enable address sanitizer" OFF)

if(WITH_SANITIZER)
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE True)
    target_compile_options(${PROJECT_NAME} INTERFACE -fsanitize=addres)
    target_link_options(${PROJECT_NAME} INTERFACE -fsanitize=address)
endif()

#examples
option(WITH_EXAMPLES "Build examples" OFF)
if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()


target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads llhttp::llhttp bro::network CLI11::CLI11 fmt::fmt uriparser::uriparser dns_resolver::dns_resolver ZLIB::ZLIB)
