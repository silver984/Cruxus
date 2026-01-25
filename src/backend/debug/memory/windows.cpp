#include <slv/backend/debug/memory/windows.hpp>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>

namespace
{
    float get_memory(float divisor)
    {
        PROCESS_MEMORY_COUNTERS pmc;

        if (K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            return static_cast<float>(pmc.WorkingSetSize) / divisor;
        }

        return 0.f;
    }
}
#endif

namespace slv
{
#ifdef _WIN32
	namespace win32
	{
        float get_memory_kb()
        {
            return get_memory(1024.f);
        }

        float get_memory_mb()
        {
            return get_memory(1024.f * 1024.f);
        }

        float get_memory_gb()
        {
            return get_memory(1024.f * 1024.f * 1024.f);
        }
    }
#endif
}