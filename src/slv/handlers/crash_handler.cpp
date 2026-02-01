#include <slv/handlers/crash_handler.hpp>
#ifdef _WIN32
#include <platform/windows/crash.hpp>
#endif

namespace slv
{
	bool CrashHandler::init()
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

		return false;
	}
}