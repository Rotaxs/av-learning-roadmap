cmake_minimum_required(VERSION 3.10)

project(addTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(myFunction)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC myFunction)

# target_include_directories(${PROJECT_NAME} 
#     PUBLIC
#         "${PROJECT_SOURCE_DIR}/myFunction"
# )

message(STATUS out --- ${PROJECT_SOURCE_DIR})
