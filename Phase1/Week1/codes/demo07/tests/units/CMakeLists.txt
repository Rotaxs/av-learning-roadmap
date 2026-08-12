# add_executable(test_myMath)
# target_sources(test_myMath
#     PRIVATE 
#         test_myMath.cpp
# )
# target_link_libraries(test_myMath
#     PRIVATE 
#         myMath
# )
# add_test(
#     NAME test_myMath_unit 
#     COMMAND test_myMath
# )

# add_executable(test_myUtils)
# target_sources(test_myUtils
#     PRIVATE 
#         test_myUtils.cpp
# )
# target_link_libraries(test_myUtils
#     PRIVATE 
#         myMath
# )
# add_test(
#     NAME test_myUtils_unit 
#     COMMAND test_myUtils
# )

# 1. 整理所有的测试源文件列表
set(TEST_SOURCES
    test_myMath.cpp
    test_myUtils.cpp
)

# 2. 循环自动创建 target 并注册到 ctest
foreach(SOURCE_FILE ${TEST_SOURCES})
    # 从文件名提取 target 名称（例如 test_myMath.cpp -> test_myMath）
    get_filename_component(TEST_NAME ${SOURCE_FILE} NAME_WE)

    # 创建可执行文件
    add_executable(${TEST_NAME} ${SOURCE_FILE})

    # 链接静态库
    target_link_libraries(${TEST_NAME} PRIVATE myMath)

    # 注册到 CTest（测试名为 target 名加上 _unit）
    add_test(NAME ${TEST_NAME}_unit COMMAND ${TEST_NAME})
endforeach()