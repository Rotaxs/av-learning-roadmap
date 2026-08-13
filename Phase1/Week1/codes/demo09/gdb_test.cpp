#include <iostream>

int arr[] = {1, 2, 2, 3 ,5, 3};

void func() {
    for (int i = 1; i <= 10; ++i) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

int main() {
    func();
    return 0;
}