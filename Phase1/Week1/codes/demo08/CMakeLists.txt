cmake_minimum_required(VERSION 3.21)
project(demo08)

add_executable(demo08 src/main.cpp src/utils.cpp)
target_include_directories(demo08 PUBLIC include)

enable_testing()
add_executable(test_add tests/test_add.cpp src/utils.cpp)
target_include_directories(test_add PUBLIC include)
add_test(
    NAME add_test
    COMMAND test_add 1 2
)

add_executable(test_cli tests/test_cli.cpp src/utils.cpp)
target_include_directories(test_cli PUBLIC include)
add_test(
    NAME cli_test
    COMMAND test_cli 1 2
)

add_test(
    NAME cli_test_no_arg
    COMMAND test_cli
)
add_test(
    NAME cli_test_exceed
    COMMAND test_cli 1 2 3
)

set_tests_properties(cli_test_exceed cli_test_no_arg
    PROPERTIES
        WILL_FAIL TRUE
)


