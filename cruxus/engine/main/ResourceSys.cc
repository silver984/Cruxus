#include <cruxus/engine/main/ResourceSys.hh>
#include <cruxus/engine/debug/log.hh>
#include <miniaudio.h>
#include <fmt/format.h>
#include <tinyxml2.h>
#include <raylib.h>
#include <filesystem>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include "parsed_path.hh"
#include "rl.hh"

namespace crx {

// private
ResourceSys::ResourceSys() :
    since_cleanup_(0.f)
{
    using enum format_type;

    auto& image_formats = supported_formats_[image];
    image_formats.reserve(3);
    image_formats.emplace_back("png");
    image_formats.emplace_back("jpg");
    image_formats.emplace_back("jpeg");

    auto& audio_formats = supported_formats_[audio];
    audio_formats.reserve(2);
    audio_formats.emplace_back("mp3");
    audio_formats.emplace_back("wav");

    auto& data_formats = supported_formats_[data];
    data_formats.reserve(1);
    data_formats.emplace_back("xml");
}

// private
ResourceSys::~ResourceSys() = default;

sptr<texture> ResourceSys::load_texture(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_textures_.find(abs_path);
        it != cached_textures_.end()
    ) {
        return it->second;
    }

    using enum format_type;
    const auto& ext = parsed.extension;
    if (!is_format_supported(image, ext)) {
        log_unsupported_format(ext, abs_path);
        return nullptr;
    }

    auto tex = rl::load_texture_stb(abs_path.c_str());
    if (!tex) {
        log_fail(abs_path);
        return nullptr;
    }

    auto [it, _] = cached_textures_.emplace(abs_path, shared<texture>(*tex));
    return it->second;
}

/*
sptr<atlas_data> ResourceSys::load_atlas_data(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_atlas_datas_.find(abs_path);
        it != cached_atlas_datas_.end()
    ) {
        return it->second;
    }

    const auto& ext = parsed.extension;
    if (!is_format_supported(DATA, ext)) {
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
        log::error(fmt::format("Can't load atlas data with unsupported data. | path: \"{}\"", abs_path));
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
    return it->second;
}
*/

sptr<std::vector<float>> ResourceSys::load_pcm_data(std::string_view path) {
    auto parsed = parsed_path::parse(path);
    const auto& abs_path = parsed.stitched;

    if (
        auto it = cached_pcm_datas_.find(abs_path);
        it != cached_pcm_datas_.end()
    ) {
        return it->second;
    }

    using enum format_type;
    const std::string& ext = parsed.extension;
    if (!is_format_supported(audio, ext)) {
        log_unsupported_format(ext, abs_path);
        return nullptr;
    }

    ma_decoder decoder;
    ma_decoder_config config = ma_decoder_config_init(
        ma_format_f32,
        CRUXUS_AUDIO_CHANNELS,
        CRUXUS_AUDIO_SAMPLE_RATE
    );

    if (
        ma_result result = ma_decoder_init_file(abs_path.c_str(), &config, &decoder);
        result != MA_SUCCESS
    ) {
        log_fail(abs_path);
        log::error(fmt::format("ma_decoder_init_file -> ma_result: {}", static_cast<int>(result)));
        return nullptr;
    }

    ma_uint64 total_frames = 0;
    ma_uint64 total_frames_read = 0;
    ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);
    auto pcm = shared<std::vector<float>>();
    pcm->resize(static_cast<size_t>(total_frames * CRUXUS_AUDIO_CHANNELS));

    while (total_frames_read < total_frames) {
        ma_uint64 frames_read = 0;
        ma_result result = ma_decoder_read_pcm_frames(
            &decoder,
            pcm->data() + total_frames_read * CRUXUS_AUDIO_CHANNELS,
            total_frames - total_frames_read,
            &frames_read
        );

        if (result != MA_SUCCESS || frames_read == 0) {
            log::warning(fmt::format("ma_decoder_read_pcm_frames -> ma_result: {}", static_cast<int>(result)));
            break;
        }

        total_frames_read += frames_read;
    }

    ma_decoder_uninit(&decoder);

    // shrink if decoder returned fewer frames than expected
    pcm->resize(static_cast<size_t>(total_frames_read * CRUXUS_AUDIO_CHANNELS));
    auto [it, _] = cached_pcm_datas_.emplace(abs_path, pcm);
    return it->second;
}

// private
void ResourceSys::update(float dt) {
    since_cleanup_ += dt;
    static float cleanup_interval = 60.f;
    while (since_cleanup_ >= cleanup_interval) {
        clean_cache();
        since_cleanup_ -= cleanup_interval;
    }
}

// private
void ResourceSys::clean_cache() {
    auto clean = [](auto& map) {
        for (auto it = map.begin(); it != map.end();) {
            if (it->second.use_count() <= 1) {
                it = map.erase(it);
            } else {
                ++it;
            }
        }
        };

    clean(cached_textures_);
    // clean(cached_atlas_datas_);
    clean(cached_pcm_datas_);
}

// private
bool ResourceSys::is_format_supported(format_type type, std::string_view format) {
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
void ResourceSys::log_unsupported_format(
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
void ResourceSys::log_fail(
    std::string_view path,
    std::source_location const& loc
) {
    log::error(fmt::format("Failed to load: \"{}\"", path), loc);
}

}