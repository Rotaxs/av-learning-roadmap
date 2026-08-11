cmake_minimum_required(VERSION 3.20)

project(textstats VERSION 1.0)

# ========== core target ===============

add_library(core STATIC)

target_sources(core
    PRIVATE
        src/stats.cpp
)

target_include_directories(core 
    PUBLIC 
        include/
)

target_compile_features(core
    PUBLIC # 使得 textstats 可以继承
        cxx_std_17
)

target_compile_options(core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

# ========== execute target ============

add_executable(textstats)

target_sources(textstats
    PRIVATE
        src/main.cpp
)

target_compile_options(textstats
    PRIVATE 
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

target_link_libraries(textstats
    PRIVATE
        core
)
