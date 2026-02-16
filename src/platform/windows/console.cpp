#include <platform/windows/console.hpp>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>

namespace slv::win32
{
    bool create_console(const std::string& title_prefix)
    {
        if (GetConsoleWindow())
        {
            return true;
        }

        AllocConsole();
        slv::win32::rename_console(title_prefix);

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        std::ios::sync_with_stdio(true);

        HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h_out == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD mode = 0;
        if (!GetConsoleMode(h_out, &mode))
        {
            return false;
        }

        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        SetConsoleMode(h_out, mode);

        return true;
    }

    void destroy_console()
    {
        if (GetConsoleWindow())
        {
            FreeConsole();
        }
    }

    void rename_console(const std::string& title_prefix)
    {
        if (GetConsoleWindow())
        {
            std::string title = "\"" + title_prefix + "\" Debug Console";
            SetConsoleTitleA(title.c_str());
        }
    }

    bool is_console_open()
    {
        return GetConsoleWindow() != nullptr;
    }
}