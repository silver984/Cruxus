#pragma once

#include <fmt/format.h>
#include <fmt/color.h>
#include <chrono>
#include <ctime>
#include <string>
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
		inline fmt::color get_level_color(slv::log level)
		{
			switch (level)
			{
			case slv::log::trace: return fmt::color::lemon_chiffon;
			case slv::log::info: return fmt::color::green_yellow;
			case slv::log::warning: return fmt::color::gold;
			case slv::log::error: return fmt::color::crimson;
			default: return fmt::color::white;
			}
		}

		inline const char* get_level_label(slv::log level)
		{
			switch (level)
			{
			case slv::log::trace: return "Trace";
			case slv::log::info: return "Info";
			case slv::log::warning: return "Warning";
			case slv::log::error: return "Error";
			default: return "Unknown";
			}
		}

		inline std::string get_time()
		{
			using clock = std::chrono::system_clock;
			const auto now = clock::now();
			const auto t = clock::to_time_t(now);
			const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

			std::tm tm{};
#ifdef _WIN32
			localtime_s(&tm, &t);
#else
			localtime_r(&t, &tm);
#endif

			char buf[13]; // HH:MM:SS.mmm\0
			std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
			std::snprintf(buf + 8, sizeof(buf) - 8, ".%03d", static_cast<int>(ms.count()));

			return std::string(buf);
		}
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

		std::string time = log_impl::get_time();
		std::string stitched_message = fmt::format(fmt::runtime(message), std::forward<args>(_args)...);

		fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time);
		fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", owner);
		fmt::print(fmt::fg(log_impl::get_level_color(level)), "[{}] ", log_impl::get_level_label(level));
		fmt::print("{}\n", stitched_message);
	}
}