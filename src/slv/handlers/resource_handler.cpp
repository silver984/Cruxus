#include <slv/handlers/resource_handler.hpp>
#include <slv/core/console_log.hpp>
#include <raylib.h>
#include <filesystem>

namespace
{
    struct parsed_path
    {
        std::filesystem::path stitched{};
        std::filesystem::path directory{};
        std::string file_name{};
        std::string extension{};
    };

    parsed_path get_parsed_path(const std::string& file_path)
    {
        std::filesystem::path abs = std::filesystem::absolute(file_path);
        
        std::string ext = abs.extension().string();
        if (!ext.empty() && ext[0] == '.')
        {
            ext.erase(0, 1); // remove the dot
        }

        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        return parsed_path
        {
            abs, // stitched
            abs.parent_path(), // directory
            abs.stem().string(), // file_name
            ext // extension
        };
    }
}

namespace slv
{
	std::shared_ptr<slv::texture> ResourceHandler::load_texture(const std::string& file_path)
	{
        parsed_path parsed = get_parsed_path(file_path);
        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_IMG_FORMATS.begin(), M_SUPPORTED_IMG_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "Can't load texture with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched.string());
            return nullptr;
        }

        std::string abs_path = parsed.stitched.string();

        if (auto it = m_cached_textures.find(abs_path); it != m_cached_textures.end())
        {
            return it->second;
        }

        Texture2D texture_rl = LoadTexture(abs_path.c_str());

        if (texture_rl.id == 0u)
        {
            slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "Failed to load texture: \"{}\"", parsed.stitched.string());
            return nullptr;
        }

        std::shared_ptr<slv::texture> tex = std::make_shared<slv::texture>();
        tex->id = texture_rl.id;
        tex->width = texture_rl.width;
        tex->height = texture_rl.height;
        tex->mipmaps = texture_rl.mipmaps;
        tex->format = texture_rl.format;

        m_cached_textures.emplace(abs_path, tex);
        slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Loaded texture: \"{}\"", parsed.stitched.string());

        return tex;
	}

    void ResourceHandler::update()
    {
        // clean up textures
        for (auto it = m_cached_textures.begin(); it != m_cached_textures.end(); /**/)
        {
            if (it->second.use_count() <= 1)
            {
                std::string key = it->first;
                slv::texture* val = it->second.get();

                if (val)
                {
                    UnloadTexture(Texture(val->id, val->width, val->height, val->mipmaps, val->format));
                    slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Unloaded texture: \"{}\"", key);
                }

                it = m_cached_textures.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}