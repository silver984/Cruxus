#include <slv/types/primitives.hpp>
#include <cmath>
#include <cstddef>

namespace slv {

rgb rgb::operator+(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c + v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return {
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    };
}

rgb rgb::operator-(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c - v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return {
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    };
}

rgb rgb::operator-() const {
    return {
        static_cast<uint8_t>(255 - r),
        static_cast<uint8_t>(255 - g),
        static_cast<uint8_t>(255 - b)
    };
}

rgb rgb::operator*(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c * v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return {
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    };
}

rgb rgb::operator/(rgb const& rhs) const {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c / v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    return {
        evaluate(r, rhs.r),
        evaluate(g, rhs.g),
        evaluate(b, rhs.b)
    };
}

rgb& rgb::operator+=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c + v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator-=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c - v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator*=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c * v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

rgb& rgb::operator/=(rgb const& rhs) {
    auto evaluate = [](uint8_t c, uint8_t v) {
        int result = c / v;
        return static_cast<uint8_t>(std::clamp(result, 0, 255));
        };

    r = evaluate(r, rhs.r);
    g = evaluate(g, rhs.g);
    b = evaluate(b, rhs.b);
    return *this;
}

bool rgb::operator==(rgb const& rhs) const {
    return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool rgb::operator!=(rgb const& rhs) const {
    return !(*this == rhs);
}

bool rgb::operator<=(rgb const& rhs) const {
    return r <= rhs.r && g <= rhs.g && b <= rhs.b;
}

bool rgb::operator>=(rgb const& rhs) const {
    return r >= rhs.r && g >= rhs.g && b >= rhs.b;
}

bool rgb::operator<(rgb const& rhs) const {
    return r < rhs.r && g < rhs.g && b < rhs.b;
}

bool rgb::operator>(rgb const& rhs) const {
    return r > rhs.r && g > rhs.g && b > rhs.b;
}

mat3::mat3() {
    m[0][0] = 1; m[0][1]; m[0][2];
    m[1][0]; m[1][1] = 1; m[1][2];
    m[2][0]; m[2][1]; m[2][2] = 1;
}

mat3::mat3(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22
) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
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

mat3 mat3::identity() {
    return {
        1.f, 0.f, 0,
        0.f, 1.f, 0,
        0.f, 0.f, 1
    };
}

mat3 mat3::translation(vec2<float> const& t) {
    return {
        1.f, 0.f, t.x,
        0.f, 1.f, t.y,
        0.f, 0.f, 1.f
    };
}

mat3 mat3::scale(vec2<float> const& s) {
    return {
        s.x, 0.f, 0,
        0.f, s.y, 0,
        0.f, 0.f, 1.f
    };
}

vec2<float> mat3::translation() const {
    return {
        m[0][2],
        m[1][2]
    };
}

mat3 mat3::operator*(mat3 const& o) const {
    mat3 r;

    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 3; ++col) {
            r.m[row][col] =
                m[row][0] * o.m[0][col] +
                m[row][1] * o.m[1][col] +
                m[row][2] * o.m[2][col];
        }
    }

    return r;
}

vec2<float> mat3::transform_point(vec2<float> const& p) const {
    return {
        p.x * m[0][0] + p.y * m[0][1] + m[0][2],
        p.x * m[1][0] + p.y * m[1][1] + m[1][2]
    };
}

}