#include <slv/core/console/is_console_open.hpp>
#include <platform/windows/console.hpp>

namespace slv::log
{
	bool is_console_open()
	{
#ifdef _WIN32
		return slv::win32::is_console_open();
#else
#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
		return true;
#endif
#endif
		return false;
	}
}