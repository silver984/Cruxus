#pragma once
#include <fmt/format.h>
#ifdef _WIN32
#include <fmt/color.h>
#endif
#include <string>
#include <utility>

namespace slv::log {

bool is_console_open();

namespace impl {

std::string get_time();
void print_time_and_messenger(std::string const& messenger);

#ifdef _WIN32
template<typename... Args>
void colored_print(std::string const& messenger, fmt::color level_color, std::string const& level_name, std::string const& message, Args&&... args) {
	print_time_and_messenger(messenger);
	fmt::print(fmt::fg(level_color), "[{}] ", level_name);
	fmt::print("{}\n", fmt::format(fmt::runtime(message), std::forward<Args>(args)...));
}
#endif

template<typename... Args>
void regular_print(std::string const& messenger, std::string const& level_name, std::string const& message, Args&&... args) {
	print_time_and_messenger(messenger);
	fmt::print("[{}] {}\n", level_name, fmt::format(fmt::runtime(message), std::forward<Args>(args)...));
}

}

template<typename... Args>
void trace(std::string const& messenger = "?", std::string const& message = "?", Args&&... args) {
	if (!is_console_open()) {
		return;
	}

#ifdef _WIN32
	impl::colored_print(messenger, fmt::color::lemon_chiffon, "TRACE", message, std::forward<Args>(args)...);
#else
	impl::regular_print(messenger, "TRACE", message, std::forward<Args>(args)...);
#endif
}

template<typename... Args>
void info(std::string const& messenger = "?", std::string const& message = "?", Args&&... args) {
	if (!is_console_open()) {
		return;
	}

#ifdef _WIN32
	impl::colored_print(messenger, fmt::color::green_yellow, "INFO", message, std::forward<Args>(args)...);
#else
	impl::regular_print(messenger, "INFO", message, std::forward<Args>(args)...);
#endif
}

template<typename... Args>
void warning(std::string const& messenger = "?", std::string const& message = "?", Args&&... args) {
	if (!is_console_open()) {
		return;
	}

#ifdef _WIN32
	impl::colored_print(messenger, fmt::color::gold, "WARNING", message, std::forward<Args>(args)...);
#else
	impl::regular_print(messenger, "INFO", message, std::forward<Args>(args)...);
#endif
}

template<typename... Args>
void error(std::string const& messenger = "?", std::string const& message = "?", Args&&... args) {
	if (!is_console_open()) {
		return;
	}

#ifdef _WIN32
	impl::colored_print(messenger, fmt::color::crimson, "ERROR", message, std::forward<Args>(args)...);
#else
	impl::regular_print(messenger, "INFO", message, std::forward<Args>(args)...);
#endif
}

}