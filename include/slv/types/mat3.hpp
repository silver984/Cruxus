#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/vec2.hpp>

namespace slv {

struct SLV_DLL mat3 final {
    mat3();
    mat3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    );

    static mat3 rotation(float radians);
    static mat3 skew(vec2<float> const& radians);
    static mat3 identity();
    static mat3 translation(vec2<float> const& t);
    static mat3 scale(vec2<float> const& s);
    vec2<float> transform_point(vec2<float> const& p) const;
    vec2<float> translation() const;
    mat3 operator*(const mat3& o) const;

    float m[3][3];
};

}