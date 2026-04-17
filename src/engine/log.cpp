#include <crx/engine/log.hpp>
#include <fmt/format.h>
#include <fmt/chrono.h>
#ifdef CRX_COLORED_LOGS
#include <fmt/color.h>
#endif
#include <chrono>
#include <string>
#include <random>
#include <cstdint>

namespace {

// this function is for simplifying the function name that comes from the given std::source_location
// currently only for MSVC
std::string_view function_name(std::source_location const& location) {
	std::string_view func = location.function_name();

	if (
		const size_t pos = func.find("__cdecl");
		pos != std::string_view::npos
	) {
		func.remove_prefix(pos + sizeof("__cdecl"));
		if (!func.empty() && func.front() == ' ') {
			func.remove_prefix(1);
		}
	}

	// remove parameter list
	if (
		const size_t end = func.find('(');
		end != std::string_view::npos
	) {
		func = func.substr(0, end);
	}

	// remove lambda noise
	if (
		const size_t lambda_pos = func.find("::<lambda_");
		lambda_pos != std::string_view::npos
	) {
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

void print_info(
	fmt::color* level_color,
	std::string_view level_name,
	std::source_location const& location
) {
#ifdef CRX_COLORED_LOGS
	fmt::print(fmt::fg(fmt::color::gray), "{:<12} {} ", current_time_str(), function_name(location));
	fmt::print(fmt::fg(*level_color), "[{}] ", level_name);
#else
	fmt::print(
		"{:<12} {} [{}] ",
		current_time_str(),
		function_name(location),
		level_name
	);
#endif
}

#ifdef CRX_COLORED_LOGS
void print(
	fmt::color level_color,
	std::string_view level_name,
	std::string_view message,
	std::source_location const& location
) {
	print_info(&level_color, level_name, location);
	fmt::print("{}\n", message);
}
#else
void print(
	std::string_view level_name,
	std::string_view message,
	std::source_location const& location
) {
	print_info(nullptr, level_name, location);
	fmt::print("{}\n", message);
}
#endif

}

namespace crx::log {

void trace(
	std::string_view message,
	std::source_location const& loc
) {
#ifdef CRX_COLORED_LOGS
	print(fmt::color::dark_sea_green, "TRACE", message, loc);
#else
	print("TRACE", message, loc);
#endif
}

void info(
	std::string_view message,
	std::source_location const& loc
) {
#ifdef CRX_COLORED_LOGS
	print(fmt::color::green_yellow, "INFO", message, loc);
#else
	print("INFO", message, loc);
#endif
}

void warning(
	std::string_view message,
	std::source_location const& loc
) {
#ifdef CRX_COLORED_LOGS
	print(fmt::color::gold, "WARNING", message, loc);
#else
	print("WARNING", message, loc);
#endif
}

void error(
	std::string_view message,
	std::source_location const& loc
) {
#ifdef CRX_COLORED_LOGS
	print(fmt::color::crimson, "ERROR", message, loc);
#else
	print("ERROR", message, loc);
#endif
}

}