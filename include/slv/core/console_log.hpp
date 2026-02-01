#pragma once

#include <fmt/format.h>
#include <fmt/color.h>
#include <chrono>
#include <ctime>
#include <string>
#include <string_view>
#ifdef _WIN32
#include <platform/windows/console.hpp>
#endif

namespace slv
{
	enum class log_level
	{
		Info,
		Warning,
		Error
	};

	inline constexpr log_level LOG_INFO = log_level::Info;
	inline constexpr log_level LOG_WARNING = log_level::Warning;
	inline constexpr log_level LOG_ERROR = log_level::Error;
	inline constexpr const char* LOG_NO_OWNER = nullptr;

	namespace log_impl
	{
		inline constexpr const char* UNKNOWN_OWNER = "?";

		inline fmt::color get_level_color(log_level level)
		{
			switch (level)
			{
			case log_level::Info:    return fmt::color::green_yellow;
			case log_level::Warning: return fmt::color::gold;
			case log_level::Error:   return fmt::color::crimson;
			default:                return fmt::color::white;
			}
		}

		inline const char* get_level_label(log_level level)
		{
			switch (level)
			{
			case log_level::Info:    return "Info";
			case log_level::Warning: return "Warning";
			case log_level::Error:   return "Error";
			default:                return "Unknown";
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
	inline void console_log(log_level level, const char* owner = slv::LOG_NO_OWNER, std::string_view message = "?", args&&... _args)
	{
#ifdef _WIN32
		if (!slv::win32::is_console_open())
		{
			return;
		}
#else
		return;
#endif

		const std::string time = log_impl::get_time();
		const std::string message_owner = owner != LOG_NO_OWNER ? owner : "?";
		const std::string stitched_message = fmt::format(fmt::runtime(message), std::forward<args>(_args)...);

		fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time);
		fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", message_owner);
		fmt::print(fmt::fg(log_impl::get_level_color(level)), "[{}] ", log_impl::get_level_label(level));
		fmt::print("{}\n", stitched_message);
	}
}
