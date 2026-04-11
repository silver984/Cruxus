#include <slv/engine/log.hpp>
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

}

namespace slv::log::impl {

void print_time_and_messenger(std::string_view messenger) {
#ifdef SLV_COLORED_LOGS
	fmt::print(fmt::fg(fmt::color::dim_gray), "{:<10} ", time());
	fmt::print(fmt::fg(fmt::color::light_blue), "[{}] ", messenger);
#else
	fmt::print("{:<10} [{}] ", time(), messenger);
#endif
}

}