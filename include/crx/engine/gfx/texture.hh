#pragma once
#include <crx/config.hh>
#include <crx/engine/math/size.hh>
#include <cstdint>

namespace crx {
struct CRX_DLL texture final {
    uint32_t id;
    size<int> bounds;
    int mipmaps;
    int format;
};
}