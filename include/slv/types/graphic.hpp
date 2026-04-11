#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/size.hpp>
#include <slv/types/vec2.hpp>
#include <slv/types/string_map.hpp>
#include <cstdint>
#include <vector>

namespace slv {

enum class atlas_format : int {
    FLASH_XML
};

struct atlas_frame final {
    bool is_valid = false;
    bool is_rotated = false;
    vec2<int> pos_on_sheet;
    size<int> size_on_sheet;
    vec2<int> offsets;
};

struct atlas_data final {
    atlas_format format;
    string_map<std::vector<atlas_frame>> frames;
};

struct texture final {
    uint32_t id;
    size<int> resolution;
    int mipmaps;
    int format;
};

}