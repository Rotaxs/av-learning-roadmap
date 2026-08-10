#include "textstats/stats.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

textstats::Stats textstats::analyze_file(const std::string& path) {
    std::fstream file(path, std::ios::in | std::ios::binary);
    textstats::Stats stats;
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    char ch;
    bool in_word = false;

    while (file.get(ch)) {
        ++stats.bytes;
        if (ch == '\n') {
            ++stats.lines;
        }
        if (std::isspace(static_cast<unsigned char>(ch))) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            ++stats.words;
        }
    }
    return stats;
}

