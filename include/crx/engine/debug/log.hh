#pragma once
#include <crx/config.hh>
#include <string_view>
#include <source_location>

namespace crx::log {
CRX_DLL void trace(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

CRX_DLL void debug(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

CRX_DLL void info(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

CRX_DLL void warning(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

CRX_DLL void error(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);
}