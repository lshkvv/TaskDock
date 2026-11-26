#pragma once

#include <string>

struct Suite {
    std::string candidate;
    std::string reference;
    std::string generator;
    std::string compare = "exact";
    long long timeout_ms = 2000;
    long long output_limit_bytes = 1048576;
    long long seed = 1;
    long long size = 8;
    std::string error;
};

Suite load_suite(const std::string& path);
void print_suite(const Suite& suite);
