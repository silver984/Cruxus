#pragma once
#include <slv/config.hh>
#include <string_view>
#include <source_location>

namespace slv::log {

SLV_DLL void trace(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

SLV_DLL void info(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

SLV_DLL void warning(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

SLV_DLL void error(
	std::string_view message,
	const std::source_location& loc = std::source_location::current()
);

}