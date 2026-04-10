#include <slv/engine/log.hpp>
#include <platform/windows/console.hpp>
#include <chrono>
#include <ctime>

namespace slv::log {

bool is_console_open() {
#ifdef _WIN32
	return win32::is_console_open();
#else
#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
	return true;
#endif
#endif
	return false;
}

namespace impl {

std::string get_time() {
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

void print_time_and_messenger(const std::string& messenger) {
#ifdef _WIN32
	fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", get_time());
	fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", messenger);
#else
	fmt::print("{:<10} [{}] ", get_time(), messenger);
#endif
}

}

}