#pragma once

#include <fmt/format.h>
#include <fmt/color.h>
#include <chrono>
#include <ctime>
#include <string>
#include <string_view>
#include <slv/backend/debug_console/data.hpp>

namespace slv
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};

	inline constexpr LogLevel LOG_INFO = LogLevel::Info;
	inline constexpr LogLevel LOG_WARNING = LogLevel::Warning;
	inline constexpr LogLevel LOG_ERROR = LogLevel::Error;
	inline constexpr const char* LOG_NO_OWNER = nullptr;

	namespace log_impl
	{
		inline constexpr const char* UNKNOWN_OWNER = "?";

		inline fmt::color get_level_color(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info:    return fmt::color::green_yellow;
			case LogLevel::Warning: return fmt::color::gold;
			case LogLevel::Error:   return fmt::color::crimson;
			default:                return fmt::color::white;
			}
		}

		inline const char* get_level_label(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info:    return "Info";
			case LogLevel::Warning: return "Warning";
			case LogLevel::Error:   return "Error";
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

	template<typename... Args>
	inline void debug_log(LogLevel level, const char* owner = LOG_NO_OWNER, std::string_view message = "?", Args&&... args)
	{
		if (!slv::is_console_active)
		{
			return;
		}

		const std::string time = log_impl::get_time();
		const std::string message_owner = owner != LOG_NO_OWNER ? owner : "?";
		const std::string stitched_message = fmt::format(fmt::runtime(message), std::forward<Args>(args)...);

		fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time);
		fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", message_owner);
		fmt::print(fmt::fg(log_impl::get_level_color(level)), "[{}] ", log_impl::get_level_label(level));
		fmt::print("{}\n", stitched_message);
	}
}
