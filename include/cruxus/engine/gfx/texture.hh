#pragma once
#include <slv/config.hh>
#include <slv/engine/math/size.hh>
#include <cstdint>

namespace crx {

struct SLV_DLL texture final {
    uint32_t id;
    size<int> bounds;
    int mipmaps;
    int format;
};

}