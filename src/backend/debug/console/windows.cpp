#include <slv/backend/debug/console/windows.hpp>
#include <slv/backend/debug/console/data.hpp>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include <iostream>
#include <string>

namespace slv
{
#ifdef _WIN32
    namespace win32
    {
        bool create_console(std::string_view title_prefix)
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

            slv::is_console_active = true;

            return true;
        }

        void destroy_console()
        {
            if (GetConsoleWindow())
            {
                slv::is_console_active = false;
                FreeConsole();
            }
        }

        void rename_console(std::string_view title_prefix)
        {
            if (GetConsoleWindow() && slv::is_console_active)
            {
                std::string title = "'" + std::string(title_prefix) + "' Debug Console";
                SetConsoleTitleA(title.c_str());
            }
        }
    }
#endif
}