#pragma once
#include <slv/config.hh>
#include <slv/types/pointers.hpp>
#include <slv/types/string_map.hpp>
#include <slv/types/enum_array.hpp>
#include <slv/types/graphic.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <source_location>

namespace crx {

class Game; // forward declare
class Sound; // forward declare
class SLV_DLL ResourceManager final {
	friend class Game;

private:
	ResourceManager();
	~ResourceManager();

	enum class format_type : int {
		IMAGE,
		AUDIO,
		DATA,
		count
	};

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	[[nodiscard]] sptr<texture> load_texture(std::string_view file);
	[[nodiscard]] sptr<atlas_data> load_atlas_data(std::string_view file);
	[[nodiscard]] sptr<std::vector<float>> load_pcm_data(std::string_view file);

private:
	void update(float dt);
	void clean_cache();
	[[nodiscard]] bool is_format_supported(format_type type, std::string_view format);
	void log_unsupported_format(
		std::string_view format,
		std::string_view path,
		std::source_location const& loc = std::source_location::current()
	);
	void log_fail(
		std::string_view path,
		std::source_location const& loc = std::source_location::current()
	);
	
	enum_array<std::vector<std::string>, format_type, format_type::count> supported_formats_;
	string_map<sptr<texture>> cached_textures_;
	string_map<sptr<atlas_data>> cached_atlas_datas_;
	string_map<sptr<std::vector<float>>> cached_pcm_datas_;
	float since_cleanup_;
};
}