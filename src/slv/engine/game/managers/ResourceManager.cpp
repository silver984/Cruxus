#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/engine/log.hpp>
#include <miniaudio/miniaudio.h>
#include <fmt/format.h>
#include <tinyxml2.h>
#include <raylib.h>
#include <filesystem>
#include <algorithm>
#include <cstddef>
#include <cstring>

namespace slv {

ResourceManager::parsed_path ResourceManager::parsed_path::parse(std::string_view file) {
    std::filesystem::path abs = std::filesystem::absolute(file);
    std::string ext = abs.extension().string();

    if (!ext.empty() && ext[0] == '.') {
        // remove the dot from the extension
        ext.erase(0, 1);
    }

    // make extension lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return {
        abs.string(),
        abs.parent_path().string(),
        abs.stem().string(),
        ext
    };
}

// private
ResourceManager::ResourceManager() :
    since_cleanup_(0.f) {
    using enum format_type;
    supported_formats_[IMAGE].emplace_back("png");
    supported_formats_[IMAGE].emplace_back("jpg");
    supported_formats_[IMAGE].emplace_back("jpeg");
    supported_formats_[AUDIO].emplace_back("mp3");
    supported_formats_[AUDIO].emplace_back("wav");
    supported_formats_[DATA].emplace_back("xml");
}

// private
ResourceManager::~ResourceManager() = default;

sptr<texture> ResourceManager::load_texture(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_textures_.find(abs_path);
        it != cached_textures_.end()
    ) {
        return it->second;
    }

    const auto& ext = parsed.extension;
    if (!is_format_supported(format_type::IMAGE, ext)) {
        log_unsupported_format(ext, abs_path);
        return nullptr;
    }

    auto texture_rl = LoadTexture(abs_path.c_str());
    if (texture_rl.id == 0) {
        log_fail(abs_path);
        return nullptr;
    }

    auto [it, _] = cached_textures_.emplace(
        abs_path,
        shared<texture>(
            texture_rl.id,
            size<int>(texture_rl.width, texture_rl.height),
            texture_rl.mipmaps,
            texture_rl.format
        )
    );

    log_load(abs_path);
    return it->second;
}

sptr<atlas_data> ResourceManager::load_atlas_data(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_atlas_datas_.find(abs_path);
        it != cached_atlas_datas_.end()
    ) {
        return it->second;
    }

    const auto& ext = parsed.extension;
    if (!is_format_supported(format_type::DATA, ext)) {
        log_unsupported_format(ext, abs_path);
        return nullptr;
    }

    // TODO: support other formats than just XML
    // when that happens, move XML parsing somewhere else

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(abs_path.c_str());

    if (result != tinyxml2::XML_SUCCESS) {
        log_fail(abs_path);
        return nullptr;
    }

    tinyxml2::XMLElement* root = doc.RootElement();

    if (!root) {
        log_fail(abs_path);
        return nullptr;
    }

    auto data = shared<atlas_data>();

    // Adobe Animate or Flash
    if (strcmp(root->Name(), "TextureAtlas") == 0) {
        data->format = atlas_format::FLASH_XML;
    } else {
        log::error(
            fmt::format(
                "Can't load atlas data with unsupported data. | path: \"{}\"",
                abs_path
            )
        );
        return nullptr;
    }

    switch (data->format) {
        using enum atlas_format;
    case FLASH_XML:
    {
        for (
            tinyxml2::XMLElement* elem = root->FirstChildElement("SubTexture");
            elem != nullptr;
            elem = elem->NextSiblingElement("SubTexture")
        ) {
            char const* name_attr = elem->Attribute("name");

            if (!name_attr) {
                continue;
            }

            atlas_frame frame;

            elem->QueryIntAttribute("width", &frame.size_on_sheet.width);
            elem->QueryIntAttribute("height", &frame.size_on_sheet.height);

            if (
                frame.size_on_sheet.width == 0 ||
                frame.size_on_sheet.height == 0
            ) {
                continue;
            }

            elem->QueryIntAttribute("x", &frame.pos_on_sheet.x);
            elem->QueryIntAttribute("y", &frame.pos_on_sheet.y);
            elem->QueryIntAttribute("frameX", &frame.offsets.x);
            elem->QueryIntAttribute("frameY", &frame.offsets.y);
            elem->QueryBoolAttribute("rotated", &frame.is_rotated);

            std::string full_frame_name = name_attr;
            auto split = full_frame_name.find_last_not_of("0123456789");
            if (split == std::string::npos) {
                continue;
            }

            std::string frame_name = full_frame_name.substr(0, split + 1);
            std::string index_str = full_frame_name.substr(split + 1);
            size_t frame_index = std::stoull(index_str);

            auto& current_frames = data->frames[frame_name];

            if (current_frames.size() <= frame_index) {
                current_frames.resize(frame_index + 1);
            }

            current_frames[frame_index] = frame;
            current_frames[frame_index].is_valid = true;
        }

        break;
    }
    }

    for (auto& [name, frames] : data->frames) {
        // remove empty spots in vector
        frames.erase(
            std::remove_if(
                frames.begin(),
                frames.end(),
                [](const auto& frame) {
                    return !frame.is_valid;
                }
            ),
            frames.end()
        );
    }

    auto [it, _] = cached_atlas_datas_.emplace(abs_path, data);
    log_load(abs_path);
    return it->second;
}

sptr<pcm_data> ResourceManager::load_pcm_data(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_pcm_datas_.find(abs_path);
        it != cached_pcm_datas_.end()
    ) {
        return it->second;
    }

    const std::string& ext = parsed.extension;
    if (!is_format_supported(format_type::AUDIO, ext)) {
        log_unsupported_format(ext, abs_path);
        return nullptr;
    }

    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(
        ma_format_f32,
        SLV_AUDIO_CHANNELS,
        SLV_AUDIO_SAMPLE_RATE
    );
    ma_result result = ma_decoder_init_file(abs_path.c_str(), &config, &decoder);

    if (result != MA_SUCCESS) {
        log_fail(abs_path);
        log::error(
            fmt::format(
                "ma_decoder_init_file -> ma_result: {}",
                static_cast<int>(result)
            )
        );
        return nullptr;
    }

    ma_uint64 total_frames = 0;
    ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);
    auto pcm = shared<pcm_data>();
    pcm->resize(static_cast<size_t>(total_frames * SLV_AUDIO_CHANNELS));

    ma_uint64 total_read = 0;
    while (total_read < total_frames) {
        ma_uint64 frames_read = 0;
        ma_result r = ma_decoder_read_pcm_frames(
            &decoder,
            pcm->data() + total_read * SLV_AUDIO_CHANNELS,
            total_frames - total_read,
            &frames_read
        );

        if (r != MA_SUCCESS || frames_read == 0) {
            log::warning(
                fmt::format(
                    "ma_decoder_read_pcm_frames -> ma_result: {}",
                    static_cast<int>(r)
                )
            );
            break;
        }

        total_read += frames_read;
    }

    ma_decoder_uninit(&decoder);

    // shrink if decoder returned fewer frames than expected
    pcm->resize(static_cast<size_t>(total_read * SLV_AUDIO_CHANNELS));
    auto [it, _] = cached_pcm_datas_.emplace(abs_path, pcm);
    log_load(abs_path);
    return it->second;
}

// private
void ResourceManager::update(float dt) {
    since_cleanup_ += dt;
    static float cleanup_interval = 60.f;
    while (since_cleanup_ >= cleanup_interval) {
        clean_cache();
        since_cleanup_ -= cleanup_interval;
    }
}

// private
void ResourceManager::clean_cache() {
    auto log_unload = [](std::string_view key) {
        log::trace(fmt::format("Unloaded: \"{}\"", key));
        };

    for (auto it = cached_textures_.begin(); it != cached_textures_.end();) {
        if (it->second.use_count() <= 1) {
            std::string key = it->first;
            auto& ptr = it->second;
            if (ptr) {
                UnloadTexture(Texture(ptr->id));
                log_unload(key);
            }

            it = cached_textures_.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = cached_atlas_datas_.begin(); it != cached_atlas_datas_.end();) {
        if (it->second.use_count() <= 1) {
            std::string key = it->first;
            it = cached_atlas_datas_.erase(it);
            log_unload(key);
        } else {
            ++it;
        }
    }

    for (auto it = cached_pcm_datas_.begin(); it != cached_pcm_datas_.end();) {
        if (it->second.use_count() <= 1) {
            std::string key = it->first;
            it = cached_pcm_datas_.erase(it);
            log_unload(key);
        } else {
            ++it;
        }
    }
}

// private
bool ResourceManager::is_format_supported(format_type type, std::string_view format) {
    auto& vec = supported_formats_[type];
    return std::any_of(
        vec.begin(),
        vec.end(),
        [&](const auto& e) {
            return format == e;
        }
    );
}

// private
void ResourceManager::log_unsupported_format(
    std::string_view format,
    std::string_view path,
    std::source_location const& loc
) {
    log::error(
        fmt::format(
            "Can't load unsupported format: \"{}\" | path: \"{}\"",
            format,
            path
        ),
        loc
    );
}

// private
void ResourceManager::log_fail(
    std::string_view path,
    std::source_location const& loc
) {
    log::error(fmt::format("Failed to load: \"{}\"", path), loc);
}

// private
void ResourceManager::log_load(
    std::string_view path,
    std::source_location const& loc
) {
    log::trace(fmt::format("Loaded: \"{}\"", path), loc);
}

}