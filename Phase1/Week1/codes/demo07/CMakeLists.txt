cmake_minimum_required(VERSION 3.21)

project(demo07)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE 
        src/main.cpp
)

add_subdirectory(src/myMath)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        myMath
)

enable_testing()
add_subdirectory(tests)
