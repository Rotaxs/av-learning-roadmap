#include "textstats/stats.h"

#include <iostream>
#include <string>
#include <cassert>
#include <functional>

#ifndef FIXTURES_DIR
#define FIXTURES_DIR "tests/fixtures"
#endif

void expect_stats(const std::string& path, 
                  std::size_t bytes, std::size_t lines, std::size_t words, 
                  const std::string& label) {
    const textstats::Stats s = textstats::analyze_file(path);
    const bool ok = s.bytes == bytes && s.lines == lines && s.words == words;
    if (!ok) {
        std::cerr << "[FAIL] " << label << std::endl
                  << "Expected: " << " bytes=" << bytes << " lines=" << lines << " words=" << words << std::endl
                  << "Actual  : " << " bytes=" << s.bytes << " lines=" << s.lines << " words" << s.words << std::endl;
    }
    assert(ok);
}

std::string fixture(const std::string& name) {
    return std::string(FIXTURES_DIR) + "/" + name;
}

void test_normal() {
    expect_stats(fixture("normal.txt"), 62, 6, 9, "normal");
}

void test_empty() {
    expect_stats(fixture("empty.txt"), 0, 0, 0, "empty");
}

void test_no_trailing_newline() {
    expect_stats(fixture("no_newline.txt"), 7, 1, 2, "no_trailing_newline");
}

void test_whitespace_only() {
    expect_stats(fixture("whitespace.txt"), 8, 2, 0, "whitespace_only");
}

int main(int argc, char* argv[]) {
    struct TestCase { const char* name; std::function<void()> fn; };
    const TestCase tests[] = {
        {"normal", test_normal},
        {"empty", test_empty},
        {"no_newline", test_no_trailing_newline},
        {"whitespace", test_whitespace_only},
    };
    const std::string sel = argc > 1 ? argv[1] : "all";
    for (const auto& t : tests) {
        if (sel == "all" || sel == t.name) t.fn();
    }

    return 0;
}
