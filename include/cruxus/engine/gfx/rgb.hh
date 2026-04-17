#pragma once
#include <slv/config.hh>
#include <slv/engine/math/numeric.hh>
#include <cstdint>
#include <algorithm>

namespace crx {

struct SLV_DLL rgb final {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // TODO: rework operators
};

}