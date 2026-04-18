#pragma once
#include <crx/config.hh>
#include <crx/engine/util/pointers.hh>
#include <crx/engine/util/string_map.hh>
#include <crx/engine/util/enum_array.hh>
#include <crx/engine/gfx/texture.hh>
#include <string>
#include <string_view>
#include <vector>
#include <source_location>

namespace crx {

class Game;
class CRX_DLL ResourceSys final {
	friend class Game;
private:
	ResourceSys();
	~ResourceSys();

	enum class format_type : int {
		IMAGE,
		AUDIO,
		DATA,
		count
	};
public:
	ResourceSys(const ResourceSys&) = delete;
	ResourceSys& operator=(const ResourceSys&) = delete;
	ResourceSys(ResourceSys&&) = delete;
	ResourceSys& operator=(ResourceSys&&) = delete;

	[[nodiscard]] sptr<texture> load_texture(std::string_view file);
	// TODO: change this
	// [[nodiscard]] sptr<atlas_data> load_atlas_data(std::string_view file);
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