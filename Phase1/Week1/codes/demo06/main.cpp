#include <iostream>
#include "config.h"

#ifdef USE_MY_ADD
    #include "add.h"
#endif

int main() {
#ifdef USE_MY_ADD
    std::cout << "使用 add 函数：" << add(1, 2) << std::endl;
#else
    std::cout << "直接输出：" << 3 << std::endl;
#endif
    return 0;
}