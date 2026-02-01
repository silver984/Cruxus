#pragma once

#include <string_view>

namespace slv::win32
{
	bool create_console(std::string_view title_prefix);
	void destroy_console();
	void rename_console(std::string_view title_prefix);
	bool is_console_open();
}