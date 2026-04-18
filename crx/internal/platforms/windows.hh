#pragma once

namespace crx::win32 {

bool enable_console_colors();
void init_mem_dump();
[[nodiscard]] float proc_memory_kb();
[[nodiscard]] float proc_memory_mb();
[[nodiscard]] float proc_memory_gb();

}