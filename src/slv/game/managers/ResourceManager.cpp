#include <slv/game/managers/ResourceManager.hpp>
#include <slv/core/console/log.hpp>
#include <slv/core/audio_config.hpp>
#include <raylib.h>
#include <tinyxml2.h>
#include <miniaudio/miniaudio.h>
#include <filesystem>
#include <algorithm>
#include <cstdint>

namespace slv
{
    // private
    void ResourceManager::update(float dt)
    {
        m_since_cleanup += dt;

        while (m_since_cleanup >= M_CLEANUP_INTERVAL)
        {
            clean_cache();
            m_since_cleanup -= M_CLEANUP_INTERVAL;
        }
    }

    // private
    void ResourceManager::clean_cache()
    {
        for (auto it = m_cached_textures.begin(); it != m_cached_textures.end();)
        {
            if (it->second.use_count() <= 1)
            {
                std::string key = it->first;
                auto& ptr = it->second;

                if (ptr)
                {
                    UnloadTexture(Texture(ptr->id, ptr->width, ptr->height, ptr->mipmaps, ptr->format));
                    slv::log::trace(M_NAME, "Unloaded texture: \"{}\"", key);
                }

                it = m_cached_textures.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = m_cached_atlas_datas.begin(); it != m_cached_atlas_datas.end();)
        {
            if (it->second.use_count() <= 1)
            {
                std::string key = it->first;
                it = m_cached_atlas_datas.erase(it);
                slv::log::trace(M_NAME, "Unloaded atlas data: \"{}\"", key);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = m_cached_pcm_datas.begin(); it != m_cached_pcm_datas.end();)
        {
            if (it->second.use_count() <= 1)
            {
                std::string key = it->first;
                it = m_cached_pcm_datas.erase(it);
                slv::log::trace(M_NAME, "Unloaded PCM data: \"{}\"", key);
            }
            else
            {
                ++it;
            }
        }
    }
    
    // private
    ResourceManager::parsed_path ResourceManager::get_parsed_path(const std::string& file_path) const
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

    slv::sptr<slv::texture> ResourceManager::load_texture(const std::string& file_path)
	{
        parsed_path parsed = get_parsed_path(file_path);

        if (auto it = m_cached_textures.find(parsed.stitched); it != m_cached_textures.end())
        {
            return it->second;
        }

        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_IMG_FORMATS.begin(), M_SUPPORTED_IMG_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::log::error(M_NAME, "Can't load texture with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return nullptr;
        }

        Texture2D texture_rl = LoadTexture(parsed.stitched.c_str());

        if (texture_rl.id == 0U)
        {
            slv::log::error(M_NAME, "Failed to load texture: \"{}\"", parsed.stitched);
            return nullptr;
        }

        slv::sptr<slv::texture> tex = std::make_shared<slv::texture>(texture_rl.id, texture_rl.width, texture_rl.height, texture_rl.mipmaps, texture_rl.format);
        m_cached_textures.emplace(parsed.stitched, tex);
        slv::log::trace(M_NAME, "Loaded texture: \"{}\"", parsed.stitched);

        return tex;
	}

    slv::sptr<slv::atlas_data> ResourceManager::load_atlas_data(const std::string& file_path)
    {
        parsed_path parsed = get_parsed_path(file_path);

        if (auto it = m_cached_atlas_datas.find(parsed.stitched); it != m_cached_atlas_datas.end())
        {
            return it->second;
        }

        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_DATA_FORMATS.begin(), M_SUPPORTED_DATA_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::log::error(M_NAME, "Can't load atlas data with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return nullptr;
        }

        auto err = [&parsed]() { slv::log::error(M_NAME, "Failed to load atlas data: \"{}\"", parsed.stitched); };

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

        slv::sptr<slv::atlas_data> atlas_data = std::make_shared<slv::atlas_data>();

        if (std::string(root->Name()) == "TextureAtlas") // Adobe Animate or Flash
        {
            atlas_data->format = slv::atlas_format::flash_xml;
        }
        else
        {
            slv::log::error(M_NAME, "Can't load atlas data with unsupported data. | file_path: \"{}\"", parsed.stitched);
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
        slv::log::trace(M_NAME, "Loaded atlas data: \"{}\"", parsed.stitched);
        return atlas_data;
    }

    slv::sptr<slv::pcm_data> ResourceManager::load_pcm_data(const std::string& file_path)
    {
        parsed_path parsed = get_parsed_path(file_path);

        if (auto it = m_cached_pcm_datas.find(parsed.stitched); it != m_cached_pcm_datas.end())
        {
            return it->second;
        }

        const std::string& ext = parsed.extension;
        bool is_format_supported = std::any_of(M_SUPPORTED_AUDIO_FORMATS.begin(), M_SUPPORTED_AUDIO_FORMATS.end(),
                                               [&](auto e) { return ext == e; });
        if (!is_format_supported)
        {
            slv::log::error(M_NAME, "Can't load PCM data with unsupported format: \"{}\" | file_path: \"{}\"", ext, parsed.stitched);
            return nullptr;
        }

        ma_format format = SLV_AUDIO_SAMPLEFORMAT;
        ma_uint32 channels = slv::AUDIO_CHANNELS;
        ma_uint32 rate = slv::AUDIO_SAMPLE_RATE;

        ma_decoder decoder;
        ma_decoder_config config = ma_decoder_config_init(format, channels, rate);
        ma_result result = ma_decoder_init_file(parsed.stitched.c_str(), &config, &decoder);

        if (result != MA_SUCCESS)
        {
            slv::log::error(M_NAME, "Failed to load PCM data | file_path: \"{}\"", parsed.stitched);
            slv::log::error(M_NAME, "ma_decoder_init_file -> ma_result: {}", static_cast<int>(result));
            return nullptr;
        }

        ma_uint64 total_frames = 0;
        ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);

        slv::sptr<slv::pcm_data> pcm = slv::shared<slv::pcm_data>();
        pcm->resize(static_cast<size_t>(total_frames * channels));

        ma_uint64 total_read = 0;

        while (total_read < total_frames)
        {
            ma_uint64 frames_read = 0;
            ma_result r = ma_decoder_read_pcm_frames(&decoder, pcm->data() + total_read * channels, total_frames - total_read, &frames_read);

            if (r != MA_SUCCESS || frames_read == 0)
            {
                slv::log::warning(M_NAME, "ma_decoder_read_pcm_frames -> ma_result: {}", static_cast<int>(r));
                break;
            }

            total_read += frames_read;
        }

        ma_decoder_uninit(&decoder);

        // shrink if decoder returned fewer frames than expected
        pcm->resize(static_cast<size_t>(total_read * channels));
        m_cached_pcm_datas.emplace(parsed.stitched, pcm);
        slv::log::trace(M_NAME, "Loaded PCM data: \"{}\" ({} frames)", parsed.stitched, static_cast<uint64_t>(total_read));
        return pcm;
    }
}