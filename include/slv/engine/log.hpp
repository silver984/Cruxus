#pragma once
#include <slv/internal/config.hpp>
#include <fmt/format.h>
#ifdef SLV_COLORED_LOGS
#include <fmt/color.h>
#endif
#include <string_view>
#include <utility>
#include <source_location>

namespace slv::log::impl {

void print_time_and_messenger(std::string_view messenger);

#ifdef SLV_COLORED_LOGS
template<typename... va_args>
void colored_print(
	std::string_view messenger,
	std::string_view level_name,
	std::string_view message,
	fmt::color level_color,
	va_args&&... args
) {
	print_time_and_messenger(messenger);
	fmt::print(fmt::fg(level_color), "[{}] ", level_name);
	fmt::print(
		"{}\n",
		fmt::format(fmt::runtime(message),
			std::forward<va_args>(args)...
		)
	);
}
#endif

template<typename... va_args>
void regular_print(
	std::string_view messenger,
	std::string_view level_name,
	std::string_view message,
	va_args&&... args
) {
	print_time_and_messenger(messenger);
	fmt::print(
		"[{}] {}\n",
		level_name,
		fmt::format(
			fmt::runtime(message),
			std::forward<va_args>(args)...
		)
	);
}

}

namespace slv::log {

template<typename... va_args>
void trace(
	std::string_view message,
	va_args&&... args,
	const std::source_location& loc = std::source_location::current()
) {
#ifdef SLV_COLORED_LOGS
	impl::colored_print(
		loc.function_name(),
		"TRACE",
		message,
		fmt::color::lemon_chiffon,
		std::forward<va_args>(args)...
	);
#else
	impl::regular_print(
		loc.function_name(),
		"TRACE",
		message,
		std::forward<va_args>(args)...
	);
#endif
}

template<typename... va_args>
void info(
	std::string_view message,
	va_args&&... args,
	const std::source_location& loc = std::source_location::current()
) {
#ifdef SLV_COLORED_LOGS
	impl::colored_print(
		loc.function_name(),
		"INFO",
		message,
		fmt::color::green_yellow,
		std::forward<va_args>(args)...
	);
#else
	impl::regular_print(
		loc.function_name(),
		"INFO",
		message,
		std::forward<va_args>(args)...
	);
#endif
}

template<typename... va_args>
void warning(
	std::string_view message,
	va_args&&... args,
	const std::source_location& loc = std::source_location::current()
) {
#ifdef SLV_COLORED_LOGS
	impl::colored_print(
		loc.function_name(),
		"WARNING",
		message,
		fmt::color::gold,
		std::forward<va_args>(args)...
	);
#else
	impl::regular_print(
		loc.function_name(),
		"INFO",
		message,
		std::forward<va_args>(args)...
	);
#endif
}

template<typename... va_args>
void error(
	std::string_view message,
	va_args&&... args,
	const std::source_location& loc = std::source_location::current()
) {
#ifdef SLV_COLORED_LOGS
	impl::colored_print(
		loc.function_name(),
		"ERROR",
		message,
		fmt::color::crimson,
		std::forward<va_args>(args)...
	);
#else
	impl::regular_print(
		loc.function_name(),
		"INFO",
		message,
		std::forward<va_args>(args)...
	);
#endif
}

}