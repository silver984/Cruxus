#pragma once

#include <slv/core/types/primitives.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace slv
{
    enum class atlas_format
    {
        flash_xml = 0
    };

    struct atlas_frame
    {
        bool is_valid = false;
        bool is_rotated = false;
        slv::vec2<int> pos_on_sheet;
        slv::size<int> size_on_sheet;
        slv::vec2<int> offsets;
    };

    struct atlas_data
    {
        slv::atlas_format format;
        std::unordered_map<std::string, std::vector<slv::atlas_frame>> frames;
    };
}