cmake_minimum_required(VERSION 3.3.2)
project(http_client_loader VERSION 1.0.0 DESCRIPTION "http simple client example")

include("${PROJECT_SOURCE_DIR}/third_party/cli11.cmake")
include("${PROJECT_SOURCE_DIR}/third_party/yaml-cpp.cmake")
include("${PROJECT_SOURCE_DIR}/third_party/quill.cmake")
find_package(Threads REQUIRED)

# core h files
set(H_FILES
    include/client_loader/config.h
    include/client_loader/connection.h
    include/client_loader/response_parser.h
    include/client_loader/loader.h
    include/client_loader/manager.h
)

# core cpp files
set(CPP_FILES
    source/config.cpp
    source/loader.cpp
    source/manager.cpp
    source/response_parser.cpp
    source/main.cpp
)

add_executable(${PROJECT_NAME} ${CPP_FILES} ${H_FILES} )

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_link_libraries(${PROJECT_NAME} PUBLIC http_client::http_client CLI11::CLI11 yaml-cpp::yaml-cpp quill::quill)
