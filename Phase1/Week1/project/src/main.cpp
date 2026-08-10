#include "textstats/stats.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file path>" << std::endl;
        return 1;
    }
    try {
    std::string filepath = argv[1];
    textstats::Stats stats = textstats::analyze_file(filepath);
    std::cout << "loading file: " << filepath << std::endl;
    std::cout << "bytes=" << stats.bytes 
              << " words=" << stats.words 
              << " lines=" << stats.lines 
              << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
