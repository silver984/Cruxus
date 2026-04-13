#include <slv/engine/log.hpp>
#include <fmt/format.h>
#include <fmt/chrono.h>
#ifdef SLV_COLORED_LOGS
#include <fmt/color.h>
#endif
#include <chrono>
#include <string>

namespace {

// this function is for simplifying the function name that comes from the given std::source_location
// currently only for MSVC
std::string_view function_name(const std::source_location& location) {
	std::string_view func = location.function_name();

	size_t pos = func.find("__cdecl");
	if (pos != std::string_view::npos) {
		func.remove_prefix(pos + sizeof("__cdecl"));
		if (!func.empty() && func.front() == ' ') {
			func.remove_prefix(1);
		}
	}

	// remove parameter list
	size_t end = func.find('(');
	if (end != std::string_view::npos) {
		func = func.substr(0, end);
	}

	// remove lambda noise
	size_t lambda_pos = func.find("::<lambda_");
	if (lambda_pos != std::string_view::npos) {
		func = func.substr(0, lambda_pos);
	}

	return func;
}

std::string current_time_str() {
	const auto now = std::chrono::system_clock::now();
	const auto seconds = floor<std::chrono::seconds>(now);
	const auto ms = duration_cast<std::chrono::milliseconds>(now - seconds).count();
	return fmt::format("{:%H:%M:%S}.{:03}", seconds, ms);
}

void print_time_and_location(std::source_location const& location) {
#ifdef SLV_COLORED_LOGS
	fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", current_time_str(), function_name(location));
	fmt::print("[{}] ", function_name(location));
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
	fmt::print("{}\n", message);
}
#else
void regular_print(
	std::source_location const& location,
	std::string_view level_name,
	std::string_view message
) {
	print_time_and_location(location);
	fmt::print("[{}] {}\n", level_name, message);
}
#endif

}

namespace slv::log {

void trace(
	std::string_view message,
	const std::source_location& loc
) {
#ifdef SLV_COLORED_LOGS
	colored_print(loc, "TRACE", message, fmt::color::dark_sea_green);
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