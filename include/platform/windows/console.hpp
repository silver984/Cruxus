#pragma once

#include <string>

namespace slv::win32
{
	bool create_console(const std::string& title_prefix);
	void destroy_console();
	void rename_console(const std::string& title_prefix);
	bool is_console_open();
}