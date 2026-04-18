#pragma once
#include <crx/config.hh>
#include <crx/engine/math/numeric.hh>
#include <cstdint>
#include <algorithm>

namespace crx {
struct CRX_DLL rgb final {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // TODO: rework operators
};
}