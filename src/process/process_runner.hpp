#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <string>

struct RunRequest {
    std::wstring application;
    std::wstring command_line;
    std::wstring working_directory;
};

struct RunResult {
    bool started = false;
    DWORD exit_code = 0;
    DWORD win_error = 0;
};

RunResult run_process(const RunRequest& request);
