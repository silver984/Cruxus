#include <slv/handlers/resource_handler.hpp>
#include <slv/core/console_log.hpp>
#include <raylib.h>
#include <tinyxml2.h>

namespace slv
{
    // private
    void ResourceHandler::update(float dt)
    {
        m_since_cleanup += dt;

        while (m_since_cleanup >= M_CLEANUP_INTERVAL)
        {
            // clean up textures
            for (auto it = m_cached_textures.begin(); it != m_cached_textures.end(); /**/)
            {
                if (it->second.use_count() <= 1)
                {
                    std::string key = it->first;
                    auto& ptr = it->second;

                    if (ptr)
                    {
                        UnloadTexture(Texture(ptr->id, ptr->width, ptr->height, ptr->mipmaps, ptr->format));
                        slv::console_log(slv::log::trace, M_CLASS_NAME, "Unloaded texture: \"{}\"", key);
                    }

                    it = m_cached_textures.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // clean up atlases
            for (auto it = m_cached_atlas_datas.begin(); it != m_cached_atlas_datas.end(); /**/)
            {
                if (it->second.use_count() <= 1)
                {
                    std::string key = it->first;
                    it = m_cached_atlas_datas.erase(it);
                    slv::console_log(slv::log::trace, M_CLASS_NAME, "Unloaded atlas data: \"{}\"", key);
                }
                else
                {
                    ++it;
                }
            }

            m_since_cleanup -= M_CLEANUP_INTERVAL;
        }
    }
    
    // private
    ResourceHandler::parsed_path ResourceHandler::get_parsed_path(const std::string& file_path) const
    {
        std::filesystem::path abs = std::filesystem::absolute(file_path);
        
        std::string ext = abs.extension().string();
        
        if (!ext.empty() && ext[0] == '.')
        {
            ext.erase(0, 1); // remove the dot
        }
        
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        return parsed_path(abs.string(),
                           abs.parent_path().string(),
                           abs.stem().string(),
                           ext);
    }

    s_ptr<slv::texture> ResourceHandler::load_texture(const std::string& file_path)
	{
        parsed_path parsed = get_parsed_path(file_path);
        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_IMG_FORMATS.begin(), M_SUPPORTED_IMG_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::console_log(slv::log::error, M_CLASS_NAME, "Can't load texture with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return nullptr;
        }

        if (auto it = m_cached_textures.find(parsed.stitched); it != m_cached_textures.end())
        {
            return it->second;
        }

        Texture2D texture_rl = LoadTexture(parsed.stitched.c_str());

        if (texture_rl.id == 0U)
        {
            slv::console_log(slv::log::error, M_CLASS_NAME, "Failed to load texture: \"{}\"", parsed.stitched);
            return nullptr;
        }

        s_ptr<slv::texture> tex = std::make_shared<slv::texture>(texture_rl.id,
                                                                           texture_rl.width,
                                                                           texture_rl.height,
                                                                           texture_rl.mipmaps,
                                                                           texture_rl.format);
        m_cached_textures.emplace(parsed.stitched, tex);
        slv::console_log(slv::log::trace, M_CLASS_NAME, "Loaded texture: \"{}\"", parsed.stitched);

        return tex;
	}

    s_ptr<slv::atlas_data> ResourceHandler::load_atlas_data(const std::string& file_path)
    {
        parsed_path parsed = get_parsed_path(file_path);
        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_DATA_FORMATS.begin(), M_SUPPORTED_DATA_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::console_log(slv::log::error, M_CLASS_NAME, "Can't load atlas data with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return nullptr;
        }

        if (auto it = m_cached_atlas_datas.find(parsed.stitched); it != m_cached_atlas_datas.end())
        {
            return it->second;
        }

        auto err = [&parsed]() { slv::console_log(slv::log::error, M_CLASS_NAME, "Failed to load atlas data: \"{}\"", parsed.stitched); };

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError result = doc.LoadFile(parsed.stitched.c_str());

        if (result != tinyxml2::XML_SUCCESS)
        {
            err();
            return nullptr;
        }

        tinyxml2::XMLElement* root = doc.RootElement();

        if (!root)
        {
            err();
            return nullptr;
        }

        s_ptr<slv::atlas_data> atlas_data = std::make_shared<slv::atlas_data>();

        if (std::string(root->Name()) == "TextureAtlas") // Adobe Animate or Flash
        {
            atlas_data->format = slv::atlas_format::flash_xml;
        }
        else
        {
            slv::console_log(slv::log::error, M_CLASS_NAME, "Can't load atlas data with unsupported data. | file_path: \"{}\"", parsed.stitched);
            return nullptr;
        }

        switch (atlas_data->format)
        {
            using enum slv::atlas_format;
        case flash_xml:
            for (tinyxml2::XMLElement* elem = root->FirstChildElement("SubTexture"); elem != nullptr; elem = elem->NextSiblingElement("SubTexture"))
            {
                if (!elem->Attribute("name"))
                {
                    continue;
                }

                std::string full_name = elem->Attribute("name");
                std::string name = full_name.substr(0, full_name.find_last_not_of("0123456789") + 1);
                size_t frame_index = std::stoull(full_name.substr(full_name.size() - 4));

                slv::atlas_frame frame;

                elem->QueryIntAttribute("x", &frame.pos_on_sheet.x);
                elem->QueryIntAttribute("y", &frame.pos_on_sheet.y);
                elem->QueryIntAttribute("width", &frame.size_on_sheet.width);
                elem->QueryIntAttribute("height", &frame.size_on_sheet.height);

                if (frame.size_on_sheet.width == 0 || frame.size_on_sheet.height == 0)
                {
                    continue;
                }

                elem->QueryIntAttribute("frameX", &frame.offsets.x);
                elem->QueryIntAttribute("frameY", &frame.offsets.y);
                elem->QueryBoolAttribute("rotated", &frame.is_rotated);
                
                auto& current_frames = atlas_data->frames[name];

                if (current_frames.size() <= frame_index)
                {
                    current_frames.resize(frame_index + 1);
                }

                current_frames[frame_index] = frame;
                current_frames[frame_index].is_valid = true;
            }

            for (auto& [name, frames] : atlas_data->frames)
            {
                // remove empty spots in vector
                frames.erase(std::remove_if(frames.begin(), frames.end(),
                             [](const auto& frame) { return !frame.is_valid; }),
                             frames.end());
            }

            break;
        default:
            return nullptr;
        }

        m_cached_atlas_datas.emplace(parsed.stitched, atlas_data);
        slv::console_log(slv::log::trace, M_CLASS_NAME, "Loaded atlas data: \"{}\"", parsed.stitched);

        return atlas_data;
    }

    /*
    slv::audio_decoder ResourceHandler::create_audio_decoder(const std::string& file_path)
    {
        parsed_path parsed = get_parsed_path(file_path);
        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_DATA_FORMATS.begin(), M_SUPPORTED_DATA_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::console_log(slv::log::error, M_CLASS_NAME, "Can't load audio decoder with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return slv::audio_decoder();
        }

        static unsigned int current_id = 0;
        current_id++;

        slv::audio_decoder audio_decoder;
        audio_decoder.id = current_id;
    }
    */
}