#include <slv/types/primitives.hpp>
#include <cmath>
#include <cstddef>

namespace slv {

rgb rgb::operator+(rgb const& rhs) const {
    return {
        r + rhs.r,
        g + rhs.g,
        b + rhs.b
    };
}

rgb rgb::operator-(rgb const& rhs) const {
    return {
        r - rhs.r,
        g - rhs.g,
        b - rhs.b
    };
}

rgb rgb::operator-() const {
    return {
        -r,
        -g,
        -b
    };
}

rgb rgb::operator*(rgb const& rhs) const {
    return {
        r * rhs.r,
        g * rhs.g,
        b * rhs.b
    };
}

rgb rgb::operator/(rgb const& rhs) const {
    return {
        r / rhs.r,
        g / rhs.g,
        b / rhs.b
    };
}

mat3 mat3::rotation(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return {
        c, -s, 0.f,
        s, c, 0.f,
        0.f, 0.f, 1.f
    };
}

mat3 mat3::skew(vec2<float> const& radians) {
    float tx = std::tan(radians.x);
    float ty = std::tan(radians.y);

    return {
        1.f, tx, 0.f,
        ty, 1.f, 0.f,
        0.f, 0.f, 1.f
    };
}

mat3 mat3::operator*(mat3 const& o) const {
    mat3 r;

    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 3; ++col) {
            r.m_[row][col] =
                m_[row][0] * o.m_[0][col] +
                m_[row][1] * o.m_[1][col] +
                m_[row][2] * o.m_[2][col];
        }
    }

    return r;
}

vec2<float> mat3::transform_point(vec2<float> const& p) const {
    return {
        p.x * m_[0][0] + p.y * m_[0][1] + m_[0][2],
        p.x * m_[1][0] + p.y * m_[1][1] + m_[1][2]
    };
}

}