#include "utils.hpp"

#include <string>

int add(int a, int b) {
    return a + b;
}

int parse(int argc, char* argv[], int& a, int& b) {
    if (argc != 3) return 1;
    std::string cha = argv[1], chb = argv[2];
    int number_a = 0, number_b = 0;
    for (int i = 0; i < cha.size(); ++i) {
        if (!std::isdigit(cha[i])) return 1;
        number_a = number_a * 10 + cha[i] - '0';
    }
    for (int i = 0; i < chb.size(); ++i) {
        if (!std::isdigit(chb[i])) return 1;
        number_b = number_b * 10 + chb[i] - '0';
    }
    a = number_a;
    b = number_b;
    return 0;
}
