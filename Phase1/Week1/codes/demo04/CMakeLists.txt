cmake_minimum_required(VERSION 3.10)

project(Test VERSION 1.21)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST OFF)

configure_file(testConfig.h.in testConfig.h)

add_executable(Test main.cpp)

target_include_directories(Test PUBLIC ${PROJECT_BINARY_DIR})

message(STATUS "${PROJECT_BINARY_DIR}")
message(STATUS "${Test_VERSION_MAJOR}")
message(STATUS "${Test_VERSION_MINOR}")
message(STATUS "${PROJECT_NAME}")
