#pragma once

#include <slv/core/console_log.hpp>
#include <chrono>
#include <ctime>

namespace slv::log_impl
{
	fmt::color get_level_color(slv::log level)
	{
		switch (level)
		{
		case slv::log::TRACE:
			return fmt::color::lemon_chiffon;

		case slv::log::INFO:
			return fmt::color::green_yellow;

		case slv::log::WARNING:
			return fmt::color::gold;

		case slv::log::ERROR:
			return fmt::color::crimson;

		default:
			return fmt::color::white;
		}
	}

	std::string get_level_label(slv::log level)
	{
		switch (level)
		{
		case slv::log::TRACE:
			return "Trace";

		case slv::log::INFO:
			return "Info";

		case slv::log::WARNING:
			return "Warning";

		case slv::log::ERROR:
			return "Error";

		default:
			return "Unknown";
		}
	}

	std::string get_time()
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