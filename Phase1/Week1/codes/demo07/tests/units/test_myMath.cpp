#include <cassert>
#include <iostream>
#include "myMath/myMath.hpp"

void test_add_basic() {
    assert(add(1, 2) == 3);
    assert(add(-1, 1) == 0);
}

void test_add_zero() {
    assert(add(0, 0) == 0);
}

int main() {
    std::cout << "[Running myMath tests...]" << std::endl;
    
    test_add_basic();
    test_add_zero();

    std::cout << "[ALL TESTS PASSED!]" << std::endl;
    return 0;
}