#include "process_runner.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

RunResult run_process(const RunRequest& request) {
    RunResult result;
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};

    std::wstring command = request.command_line;
    if (command.empty()) {
        command = request.application;
    }

    BOOL created = CreateProcessW(
        request.application.empty() ? nullptr : request.application.c_str(),
        command.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        request.working_directory.empty() ? nullptr : request.working_directory.c_str(),
        &startup,
        &process);
    if (!created) {
        result.win_error = GetLastError();
        return result;
    }

    WaitForSingleObject(process.hProcess, INFINITE);
    GetExitCodeProcess(process.hProcess, &result.exit_code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    result.started = true;
    return result;
}
