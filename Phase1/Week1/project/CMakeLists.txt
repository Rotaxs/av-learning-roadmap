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

# ========== testing ============

enable_testing()
add_executable(stats_test tests/stats_test.cpp)
target_link_libraries(stats_test PRIVATE core)
target_compile_definitions(stats_test PRIVATE
    FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures")

foreach(t normal empty no_newline whitespace)
    add_test(NAME ${t} COMMAND stats_test ${t})
endforeach()

add_test(NAME cli_missing_arg COMMAND textstats)
add_test(NAME cli_not_exist COMMAND textstats not_exist.txt)
set_tests_properties(cli_missing_arg cli_not_exist PROPERTIES WILL_FAIL TRUE)
