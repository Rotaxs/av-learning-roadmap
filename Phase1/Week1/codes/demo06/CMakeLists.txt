cmake_minimum_required(VERSION 3.13)

project(demo06)

option(USE_MY_ADD "是否使用 add 库" ON)

configure_file(config.h.in config.h)

add_executable(demo06 main.cpp)

target_include_directories(demo06 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(USE_MY_ADD)
    message(STATUS "开启使用 add 库")
    add_subdirectory(add)
    target_link_libraries(demo06 PUBLIC add)
endif()