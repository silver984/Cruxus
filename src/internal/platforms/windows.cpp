#include "platforms/windows.hpp"
#include <windows.h>
#include <psapi.h>
#include <stdexcept>
#include <DbgHelp.h>
#include <corecrt_wstdio.h>
#pragma comment(lib, "Dbghelp.lib")

namespace {

LONG WINAPI HandleException(EXCEPTION_POINTERS* exception_info) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    wchar_t dump_path[MAX_PATH];
    swprintf_s(dump_path, _countof(dump_path), L"crash-%04d-%02d-%02d_%02d-%02d-%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    HANDLE file = CreateFileW(dump_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION mei{};
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = exception_info;
        mei.ClientPointers = FALSE;

        MINIDUMP_TYPE dump_type = static_cast<MINIDUMP_TYPE>(MiniDumpWithFullMemory | MiniDumpWithHandleData | MiniDumpWithThreadInfo);
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, dump_type, &mei, NULL, NULL);
        CloseHandle(file);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

float proc_memory(float divisor) {
    PROCESS_MEMORY_COUNTERS pmc;

    if (K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<float>(pmc.WorkingSetSize) / divisor;
    }

    return 0.f;
}

}

namespace slv::win32 {

bool enable_console_colors() {
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_out == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dw_mode = 0;
    if (!GetConsoleMode(h_out, &dw_mode)) {
        return false;
    }

    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_out, dw_mode);
    return true;
}

void init_mem_dump() {
    SetUnhandledExceptionFilter(HandleException);
}

float proc_memory_kb() {
    return proc_memory(1024.0F);
}

float proc_memory_mb() {
    return proc_memory(1024.0F * 1024.0F);
}

float proc_memory_gb() {
    return proc_memory(1024.0F * 1024.0F * 1024.0F);
}

}