#pragma once

#include <slv/core/console/is_console_open.hpp>
#include <fmt/format.h>
#ifdef _WIN32
#include <fmt/color.h>
#endif
#include <string>
#include <utility>

namespace slv::log
{
	namespace impl
	{
		std::string get_time();
		void print_time_and_messenger(const std::string& messenger);

#ifdef _WIN32
		template<typename... args>
		void colored_print(const std::string& messenger, fmt::color level_color, const std::string& level_name, const std::string& message, args&&... _args)
		{
			print_time_and_messenger(messenger);
			fmt::print(fmt::fg(level_color), "[{}] ", level_name);
			fmt::print("{}\n", fmt::format(fmt::runtime(message), std::forward<args>(_args)...));
		}
#endif

		template<typename... args>
		void regular_print(const std::string& messenger, std::string& level_name, const std::string& message, args&&... _args)
		{
			print_time_and_messenger(messenger);
			fmt::print("[{}] {}\n", level_name, fmt::format(fmt::runtime(message), std::forward<args>(_args)...));
		}
	}

	template<typename... args>
	void trace(const std::string& messenger = "?", const std::string& message = "?", args&&... _args)
	{
		if (!is_console_open())
		{
			return;
		}

#ifdef _WIN32
		impl::colored_print(messenger, fmt::color::lemon_chiffon, "TRACE", message, std::forward<args>(_args)...);
#else
		impl::regular_print(messenger, "TRACE", message, std::forward<args>(_args)...);
#endif
	}

	template<typename... args>
	void info(const std::string& messenger = "?", const std::string& message = "?", args&&... _args)
	{
		if (!is_console_open())
		{
			return;
		}

#ifdef _WIN32
		impl::colored_print(messenger, fmt::color::green_yellow, "INFO", message, std::forward<args>(_args)...);
#else
		impl::regular_print(messenger, "INFO", message, std::forward<args>(_args)...);
#endif
	}

	template<typename... args>
	void warning(const std::string& messenger = "?", const std::string& message = "?", args&&... _args)
	{
		if (!is_console_open())
		{
			return;
		}

#ifdef _WIN32
		impl::colored_print(messenger, fmt::color::gold, "WARNING", message, std::forward<args>(_args)...);
#else
		impl::regular_print(messenger, "INFO", message, std::forward<args>(_args)...);
#endif
	}

	template<typename... args>
	void error(const std::string& messenger = "?", const std::string& message = "?", args&&... _args)
	{
		if (!is_console_open())
		{
			return;
		}

#ifdef _WIN32
		impl::colored_print(messenger, fmt::color::crimson, "ERROR", message, std::forward<args>(_args)...);
#else
		impl::regular_print(messenger, "INFO", message, std::forward<args>(_args)...);
#endif
	}
}