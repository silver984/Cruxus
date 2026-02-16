#pragma once

#include <fmt/format.h>
#include <fmt/color.h>
#include <string>
#include <utility>

#ifdef _WIN32
#include <platform/windows/console.hpp>
#endif

namespace slv
{
	enum class log
	{
		trace,
		info,
		warning,
		error
	};

	namespace log_impl
	{
		fmt::color get_level_color(slv::log level);
		std::string get_level_label(slv::log level);
		std::string get_time();
	}

	template<typename... args>
	inline void console_log(slv::log level, const std::string& owner = "?", const std::string& message = "?", args&&... _args)
	{
#ifdef _WIN32
		if (!slv::win32::is_console_open())
		{
			return;
		}
#else
		return;
#endif

		std::string time = slv::log_impl::get_time();
		std::string stitched_message = fmt::format(fmt::runtime(message), std::forward<args>(_args)...);

		fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time);
		fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", owner);
		fmt::print(fmt::fg(slv::log_impl::get_level_color(level)), "[{}] ", slv::log_impl::get_level_label(level));
		fmt::print("{}\n", stitched_message);
	}
}