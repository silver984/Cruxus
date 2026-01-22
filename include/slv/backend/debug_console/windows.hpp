#pragma once

#include <string_view>

namespace slv
{
#ifdef _WIN32
	namespace win32
	{
		bool create_console(std::string_view title_prefix);
		void destroy_console();
	}
#endif
}