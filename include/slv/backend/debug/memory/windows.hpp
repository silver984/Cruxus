#pragma once

namespace slv
{
#ifdef _WIN32
	namespace win32
	{
		// Get current memory usage in kilobytes.
		float get_memory_kb();

		// Get current memory usage in megabytes.
		float get_memory_mb();

		// Get current memory usage in gigabytes.
		float get_memory_gb();
	}
#endif
}