#pragma once

#include <string>
#include <cstddef>

namespace textstats {

struct Stats {
    std::size_t bytes{};
    std::size_t lines{};
    std::size_t words{};
};

Stats analyze_file(const std::string& path);

}