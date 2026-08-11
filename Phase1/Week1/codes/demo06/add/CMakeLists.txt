cmake_minimum_required(VERSION 3.13)

project(add VERSION 1.0)

add_library(add add.cpp)

target_include_directories(add PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
