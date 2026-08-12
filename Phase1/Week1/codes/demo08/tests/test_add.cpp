#include "utils.hpp"

#include <cassert>
#include <iostream>

int main() {
    int a = 1, b = 2;
    // 只有在有错误时才会在 ctest 日志中显示
    std::cerr << "hello a + b" << std::endl;
    assert(add(a, b) == 3);
    return 0;
}
