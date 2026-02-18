#include <slv/game/managers/CrashManager.hpp>
#include <slv/core/console/log.hpp>
#ifdef _WIN32
#include <platform/windows/crash.hpp>
#endif

namespace slv
{
	// private
	bool CrashManager::init()
	{
		if (m_is_init)
		{
			return true;
		}

#ifdef _WIN32
		slv::win32::init_mem_dump();
		m_is_init = true;
		return true;
#endif

		slv::log::warning(M_NAME, "SLV's CrashManager is not available on the current platform");

		return false;
	}
}