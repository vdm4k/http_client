cmake_minimum_required(VERSION 3.14.0)
project(http_client_test VERSION 1.0.0)

include("${PROJECT_SOURCE_DIR}/third_party/gtest.cmake")
enable_testing()

add_executable(${PROJECT_NAME} client_tests.cpp)
target_link_libraries(${PROJECT_NAME} GTest::gtest_main http_client::http_client)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

