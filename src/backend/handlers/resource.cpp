#include <slv/backend/handlers/resource.hpp>
#include <slv/debug/log.hpp>
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
        std::string& ext = parsed.extension;
        bool is_format_supported = (ext == "png" ||
                                    ext == "jpg" ||
                                    ext == "jpeg");
        if (!is_format_supported)
        {
            slv::debug_log(slv::LOG_ERROR, M_CLASS_NAME, "Can't load texture with unsupported format: '{}' | file_path: '{}'", ext, parsed.stitched.string());
            return nullptr;
        }

        std::string abs_path = parsed.stitched.string();

        if (auto it = m_cached_textures.find(abs_path); it != m_cached_textures.end())
        {
            return it->second;
        }

        Texture2D rl = LoadTexture(abs_path.c_str());

        std::shared_ptr<slv::texture> tex = std::make_shared<slv::texture>();
        tex->id = rl.id;
        tex->width = rl.width;
        tex->height = rl.height;
        tex->mipmaps = rl.mipmaps;
        tex->format = rl.format;

        m_cached_textures.emplace(abs_path, tex);
        slv::debug_log(slv::LOG_INFO, M_CLASS_NAME, "Successfully loaded and cached texture: '{}'", parsed.stitched.string());
        return tex;
	}
}