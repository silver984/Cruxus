#pragma once

#include <slv/types.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace slv
{
	class ResourceHandler
	{
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

	private:
		static constexpr const char* M_CLASS_NAME = "ResourceHandler";
		std::unordered_map<std::string, std::shared_ptr<slv::texture>> m_cached_textures;
	};
}