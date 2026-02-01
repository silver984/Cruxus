#include <platform/windows/crash.hpp>
#include <stdexcept>
#include <windows.h>
#include <DbgHelp.h>
#include <corecrt_wstdio.h>
#pragma comment(lib, "Dbghelp.lib")

namespace
{
    LONG WINAPI HandleException(EXCEPTION_POINTERS* exception_info)
    {
        SYSTEMTIME time;

        GetLocalTime(&time);

        wchar_t dump_path[MAX_PATH];

        swprintf_s(dump_path, _countof(dump_path), L"crash-%04d-%02d-%02d_%02d-%02d-%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

        HANDLE file = CreateFileW(dump_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (file != INVALID_HANDLE_VALUE)
        {
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
}

namespace slv::win32
{
	void init_mem_dump()
	{
        SetUnhandledExceptionFilter(HandleException);
	}
}