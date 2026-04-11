#pragma once
#include <slv/internal/config.hpp>

namespace slv::win32 {

bool enable_console_colors();
void init_mem_dump();
SLV_DLL [[nodiscard]] float proc_memory_kb();
SLV_DLL [[nodiscard]] float proc_memory_mb();
SLV_DLL [[nodiscard]] float proc_memory_gb();

}