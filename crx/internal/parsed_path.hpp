#pragma once
#include <string>
#include <string_view>

namespace crx {

struct parsed_path final {
	parsed_path() = default;
	explicit parsed_path(
		std::string_view stitched_val,
		std::string_view directory_val,
		std::string_view file_name_val,
		std::string_view extension_val
	);

	static parsed_path parse(std::string_view file);
	std::string stitched;
	std::string directory;
	std::string file_name;
	std::string extension;
};

}