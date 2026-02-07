#pragma once

#include <slv/core/types.hpp>
#include <slv/core/sprite_atlas.hpp>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <memory>
#include <array>

namespace slv
{
	class Game; // forward declare

	class ResourceHandler
	{
		friend class slv::Game;

	private:
		ResourceHandler() = default;
		~ResourceHandler() = default;

	public:
		static inline ResourceHandler& get()
        {
            static ResourceHandler instance;
            return instance;
        }

        ResourceHandler(const ResourceHandler&) = delete;
        ResourceHandler(ResourceHandler&&) = delete;
        ResourceHandler& operator = (const ResourceHandler&) = delete;
        ResourceHandler& operator = (ResourceHandler&&) = delete;

		std::shared_ptr<slv::texture> load_texture(const std::string& file_path);
		std::shared_ptr<slv::sprite::atlas_data> load_atlas_data(const std::string& file_path);

	private:
		struct parsed_path
		{
			std::string stitched;
			std::string directory;
			std::string file_name;
			std::string extension;
		};

		void update(float dt);
		parsed_path get_parsed_path(const std::string& file_path) const;

		static constexpr inline const char* M_CLASS_NAME = "ResourceHandler";
		static constexpr inline float M_CLEANUP_INTERVAL = 1.0F;
		static constexpr inline std::array<const char*, 3> M_SUPPORTED_IMG_FORMATS{ "png", "jpg", "jpeg" };
		static constexpr inline std::array<const char*, 1> M_SUPPORTED_DATA_FORMATS{ "xml" };
		float m_since_cleanup = 0.0F;
		std::unordered_map<std::string, std::shared_ptr<slv::texture>> m_cached_textures;
		std::unordered_map<std::string, std::shared_ptr<slv::sprite::atlas_data>> m_cached_atlas_datas;
	};
}