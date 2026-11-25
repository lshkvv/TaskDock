#include "process_runner.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>

namespace {

std::wstring quote_arg(const std::wstring& arg) {
    if (arg.empty()) {
        return L"\"\"";
    }
    if (arg.find_first_of(L" \t\"") == std::wstring::npos) {
        return arg;
    }
    std::wstring out = L"\"";
    for (wchar_t ch : arg) {
        if (ch == L'"') {
            out += L'\\';
        }
        out += ch;
    }
    out += L'"';
    return out;
}

std::vector<std::wstring> command_args() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::vector<std::wstring> args;
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    LocalFree(argv);
    return args;
}

}  // namespace

int main() {
    std::vector<std::wstring> args = command_args();
    if (args.size() < 3 || args[1] != L"run") {
        std::cout << "taskdock run <exe> [args...]\n";
        return 2;
    }

    RunRequest request;
    request.application = args[2];
    request.command_line = quote_arg(args[2]);
    for (std::size_t i = 3; i < args.size(); ++i) {
        request.command_line += L" ";
        request.command_line += quote_arg(args[i]);
    }

    RunResult result = run_process(request);
    if (!result.started) {
        std::cerr << "START_ERROR " << result.win_error << '\n';
        return 1;
    }
    std::cout << "exit " << result.exit_code << '\n';
    return result.exit_code == 0 ? 0 : 1;
}
