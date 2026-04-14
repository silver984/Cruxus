#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/size.hpp>
#include <slv/types/vec2.hpp>
#include <slv/types/string_map.hpp>
#include <cstdint>
#include <vector>

namespace slv {

enum class atlas_format : int {
    NONE,
    FLASH_XML
};

struct atlas_frame final {
    atlas_frame();
    bool is_valid;
    bool is_rotated;
    vec2<int> pos_on_sheet;
    size<int> size_on_sheet;
    vec2<int> offsets;
};

struct atlas_data final {
    atlas_data();
    atlas_format format;
    string_map<std::vector<atlas_frame>> frames;
};

struct texture final {
    texture();
    explicit texture(
        uint32_t id_val,
        size<int> bounds_val,
        int mipmaps_val,
        int format_val
    );
    uint32_t id;
    size<int> bounds;
    int mipmaps;
    int format;
};

}