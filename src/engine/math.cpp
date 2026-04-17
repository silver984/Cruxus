#pragma once
#include <slv/engine/math.hpp>

namespace crx::math {

float deg2rad(float deg) {
    return deg * (pi / 180.f);
}

float rad2deg(float rad) {
    return rad * (180.f / pi);
}

}