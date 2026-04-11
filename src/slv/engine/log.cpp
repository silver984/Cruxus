#include <slv/engine/log.hpp>
#include <fmt/format.h>
#ifdef SLV_COLORED_LOGS
#include <fmt/color.h>
#endif
#include <chrono>
#include <ctime>
#include <string>

namespace {

std::string time() {
	using clock = std::chrono::system_clock;
	const auto now = clock::now();
	const auto t = clock::to_time_t(now);
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::tm tm;
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

void print_time_and_location(std::source_location const& location) {
#ifdef SLV_COLORED_LOGS
	fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time());
	fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", location.function_name());
#else
	fmt::print("{:<10} [{}] ", time(), location.function_name());
#endif
}

#ifdef SLV_COLORED_LOGS
void colored_print(
	std::source_location const& location,
	std::string_view level_name,
	std::string_view message,
	fmt::color level_color
) {
	print_time_and_location(location);
	fmt::print(fmt::fg(level_color), "[{}] ", level_name);
	fmt::print("{}\n", fmt::runtime(message));
}
#else
void regular_print(
	std::source_location const& location,
	std::string_view level_name,
	std::string_view message
) {
	print_time_and_location(location);
	fmt::print("[{}] {}\n", level_name, fmt::runtime(message));
}
#endif

}

namespace slv::log {

void trace(
	std::string_view message,
	const std::source_location& loc
) {
#ifdef SLV_COLORED_LOGS
	colored_print(loc, "TRACE", message, fmt::color::lemon_chiffon);
#else
	regular_print(loc, "TRACE", message);
#endif
}

void info(
	std::string_view message,
	const std::source_location& loc
) {
#ifdef SLV_COLORED_LOGS
	colored_print(loc, "INFO", message, fmt::color::green_yellow);
#else
	regular_print(loc, "INFO", message);
#endif
}

void warning(
	std::string_view message,
	const std::source_location& loc
) {
#ifdef SLV_COLORED_LOGS
	colored_print(loc, "WARNING", message, fmt::color::gold);
#else
	regular_print(loc, "WARNING", message);
#endif
}

void error(
	std::string_view message,
	const std::source_location& loc
) {
#ifdef SLV_COLORED_LOGS
	colored_print(loc, "ERROR", message, fmt::color::crimson);
#else
	regular_print(loc, "ERROR", message);
#endif
}

}