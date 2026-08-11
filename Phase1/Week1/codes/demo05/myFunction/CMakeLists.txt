add_library(myFunction myAdd.cpp)

target_include_directories(myFunction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

message(STATUS inner --- ${PROJECT_SOURCE_DIR})
