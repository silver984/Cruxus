#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/string_map.hpp>
#include <slv/types/enum_array.hpp>
#include <slv/types/graphic.hpp>
#include <slv/types/memory.hpp>
#include <slv/types/audio.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace slv {

struct SLV_DLL parsed_path final {
	static parsed_path parse(std::string_view file);
	std::string stitched;
	std::string directory;
	std::string file_name;
	std::string extension;
};

enum class format_type : int {
	IMAGE,
	AUDIO,
	DATA,
	count
};

class Game; // forward declare
class SLV_DLL ResourceManager final {
	friend class Game;

private:
	ResourceManager();
	~ResourceManager();

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	[[nodiscard]] sptr<texture> load_texture(std::string_view file);
	[[nodiscard]] sptr<atlas_data> load_atlas_data(std::string_view file);
	[[nodiscard]] sptr<pcm_data> load_pcm_data(std::string_view file);

private:
	void update(float dt);
	void clean_cache();
	[[nodiscard]] bool is_format_supported(format_type type, std::string_view format);
	void log_unsupported(std::string_view format, std::string_view path);
	void log_fail(std::string_view path);
	void log_load(std::string_view path);

	enum_array<std::vector<std::string>, format_type, format_type::count> supported_formats_;
	string_map<sptr<texture>> cached_textures_;
	string_map<sptr<atlas_data>> cached_atlas_datas_;
	string_map<sptr<pcm_data>> cached_pcm_datas_;
	float since_cleanup_;
};
}