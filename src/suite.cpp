#include "suite.hpp"

#include "util.hpp"

#include <iostream>

namespace {

template <typename T>
void assign_number(const std::string& text, const char* key, T& field) {
    if (auto value = json_number(text, key)) {
        field = static_cast<T>(*value);
    }
}

}  // namespace

Suite load_suite(const std::string& path) {
    Suite suite;
    std::string error;
    std::string text = read_text_file(path, error);
    if (!error.empty()) {
        suite.error = error;
        return suite;
    }
    if (auto value = json_string(text, "candidate")) {
        suite.candidate = *value;
    }
    if (auto value = json_string(text, "reference")) {
        suite.reference = *value;
    }
    if (auto value = json_string(text, "generator")) {
        suite.generator = *value;
    }
    if (auto value = json_string(text, "compare")) {
        suite.compare = *value;
    }
    assign_number(text, "timeout_ms", suite.timeout_ms);
    assign_number(text, "output_limit_bytes", suite.output_limit_bytes);
    assign_number(text, "seed", suite.seed);
    assign_number(text, "size", suite.size);
    if (suite.candidate.empty() || suite.reference.empty()) {
        suite.error = "в suite.json нужны candidate и reference";
    }
    return suite;
}

void print_suite(const Suite& suite) {
    if (!suite.error.empty()) {
        std::cerr << suite.error << '\n';
        return;
    }
    std::cout << "candidate: " << suite.candidate << '\n';
    std::cout << "reference: " << suite.reference << '\n';
    std::cout << "generator: " << suite.generator << '\n';
    std::cout << "compare: " << suite.compare << '\n';
    std::cout << "timeout_ms: " << suite.timeout_ms << '\n';
    std::cout << "output_limit_bytes: " << suite.output_limit_bytes << '\n';
    std::cout << "seed: " << suite.seed << '\n';
    std::cout << "size: " << suite.size << '\n';
}
