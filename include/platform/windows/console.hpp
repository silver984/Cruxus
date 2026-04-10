#pragma once

namespace slv::win32 {

bool enable_console_colors();
[[nodiscard]] bool is_console_colors_enabled();
[[nodiscard]] bool is_console_open();

}