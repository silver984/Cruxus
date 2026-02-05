#pragma once

#include <slv/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace slv::sprite
{
    enum class atlas_format
    {
        flash_xml = 0
    };

    struct atlas_frame
    {
        bool is_valid = false;
        bool is_rotated = false;
        slv::vec_2<int> pos_on_sheet;
        slv::size<int> size_on_sheet;
        slv::vec_2<int> offsets;
    };

    struct atlas_data
    {
        slv::sprite::atlas_format format;
        std::unordered_map<std::string, std::vector<slv::sprite::atlas_frame>> frames;
    };
}