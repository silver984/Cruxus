#include <platform/windows/console.hpp>
#include <windows.h>

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

bool is_console_colors_enabled() {
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_out == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(h_out, &mode)) {
        return false;
    }

    return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

bool is_console_open() {
    return GetConsoleWindow() != NULL;
}

} // namespace slv::win32