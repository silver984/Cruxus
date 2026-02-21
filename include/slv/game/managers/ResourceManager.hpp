#pragma once

#include <slv/core/types/texture.hpp>
#include <slv/core/types/atlas.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/types/audio.hpp>
#include <unordered_map>
#include <string>
#include <array>

namespace slv
{
	class Game; // forward declare

	class ResourceManager
	{
		friend class slv::Game;

	private:
		ResourceManager() = default;
		~ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

	public:
		slv::sptr<slv::texture> load_texture(const std::string& file_path);
		slv::sptr<slv::atlas_data> load_atlas_data(const std::string& file_path);
		slv::sptr<slv::pcm_data> load_pcm_data(const std::string& file_path);

	private:
		struct parsed_path
		{
			std::string stitched;
			std::string directory;
			std::string file_name;
			std::string extension;
		};

		void update(float dt);
		void clean_cache();
		parsed_path get_parsed_path(const std::string& file_path) const;

		static constexpr inline const char* M_NAME = "ResourceManager";
		static constexpr inline float M_CLEANUP_INTERVAL = 1.f;
		static constexpr inline std::array<const char*, 3> M_SUPPORTED_IMG_FORMATS{ "png", "jpg", "jpeg" };
		static constexpr inline std::array<const char*, 2> M_SUPPORTED_AUDIO_FORMATS{ "mp3", "wav" };
		static constexpr inline std::array<const char*, 1> M_SUPPORTED_DATA_FORMATS{ "xml" };
		std::unordered_map<std::string, slv::sptr<slv::texture>> m_cached_textures;
		std::unordered_map<std::string, slv::sptr<slv::atlas_data>> m_cached_atlas_datas;
		std::unordered_map<std::string, slv::sptr<slv::pcm_data>> m_cached_pcm_datas;
		float m_since_cleanup = 0.f;
	};
}