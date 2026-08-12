#include <iostream>

#include "utils.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <number1> <number2>" << std::endl;
        return 1;
    }

    int a, b;
    int exit_code = parse(argc, argv, a, b);
    
    if (exit_code != 0) {
        std::cerr << "Error: Failed to parse" << std::endl;
        return 1;
    }

    int ans = add(a, b);

    std::cout << a << " + "  << b << " = " << ans << std::endl;

    return 0;
}