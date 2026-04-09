#pragma once
#include <slv/core/dll.hpp>
#include <cstdint>
#include <concepts>
#include <type_traits>

namespace slv {

template<typename T>
concept numeric = std::is_arithmetic_v<T>;

struct SLV_DLL rgb final {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb operator+(rgb const& rhs) const;
    rgb operator-(rgb const& rhs) const;
    rgb operator-() const;
    rgb operator*(rgb const& rhs) const;
    rgb operator/(rgb const& rhs) const;

    template <numeric U>
    constexpr rgb operator+(U rhs) const {
        return {
            static_cast<uint8_t>(r + rhs),
            static_cast<uint8_t>(g + rhs),
            static_cast<uint8_t>(b + rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator-(U rhs) const {
        return {
            static_cast<uint8_t>(r - rhs),
            static_cast<uint8_t>(g - rhs),
            static_cast<uint8_t>(b - rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator*(U rhs) const {
        return {
            static_cast<uint8_t>(r * rhs),
            static_cast<uint8_t>(g * rhs),
            static_cast<uint8_t>(b * rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator/(U rhs) const {
        return {
            static_cast<uint8_t>(r / rhs),
            static_cast<uint8_t>(g / rhs),
            static_cast<uint8_t>(b / rhs)
        };
    }
};

template<numeric T>
struct size final {
    T width;
    T height;

    constexpr size<T> operator+(size<T> const& rhs) const {
        return {
            width + rhs.width,
            height + rhs.height
        };
    }

    constexpr size<T> operator-(size<T> const& rhs) const {
        return {
            width - rhs.width,
            height - rhs.height
        };
    }

    constexpr size<T> operator-() const {
        return {
            -width,
            -height
        };
    }

    constexpr size<T> operator*(size<T> const& rhs) const {
        return {
            width * rhs.width,
            height * rhs.height
        };
    }

    constexpr size<T> operator/(size<T> const& rhs) const {
        return {
            width / rhs.width,
            height / rhs.height
        };
    }

    template<numeric U>
    constexpr size<T> operator+(U rhs) const {
        return {
            width + rhs,
            height + rhs
        };
    }

    template<numeric U>
    constexpr size<T> operator-(U rhs) const {
        return {
            width - rhs,
            height - rhs
        };
    }

    template<numeric U>
    constexpr size<T> operator*(U rhs) const {
        return {
            width * rhs,
            height * rhs
        };
    }

    template<numeric U>
    constexpr size<T> operator/(U rhs) const {
        return {
            width / rhs,
            height / rhs
        };
    }

    constexpr size<T>& operator+=(size<T> const& rhs) {
        width += rhs.width;
        height += rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator+=(U rhs) {
        width += rhs;
        height += rhs;
        return *this;
    }

    constexpr size<T>& operator-=(size<T> const& rhs) {
        width -= rhs.width;
        height -= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator-=(U rhs) {
        width -= rhs;
        height -= rhs;
        return *this;
    }

    constexpr size<T>& operator*=(size<T> const& rhs) {
        width *= rhs.width;
        height *= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator*=(U rhs) {
        width *= rhs;
        height *= rhs;
        return *this;
    }

    constexpr size<T>& operator/=(size<T> const& rhs) {
        width /= rhs.width;
        height /= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator/=(U rhs) {
        width /= rhs;
        height /= rhs;
        return *this;
    }

    constexpr bool operator==(size<T> const& rhs) const {
        return width == rhs.width && height == rhs.height;
    }

    constexpr bool operator!=(size<T> const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator<=(size<T> const& rhs) const {
        return width <= rhs.width && height <= rhs.height;
    }

    constexpr bool operator>=(size<T> const& rhs) const {
        return width >= rhs.width && height >= rhs.height;
    }

    constexpr bool operator<(size<T> const& rhs) const {
        return width < rhs.width && height < rhs.height;
    }

    constexpr bool operator>(size<T> const& rhs) const {
        return width > rhs.width && height > rhs.height;
    }
};

template<numeric T>
struct vec2 {
    T x;
    T y;

    constexpr vec2<T> operator+(vec2<T> const& rhs) const {
        return {
            x + rhs.x,
            y + rhs.y
        };
    }

    constexpr vec2<T> operator-(vec2<T> const& rhs) const {
        return {
            x - rhs.x,
            y - rhs.y
        };
    }

    constexpr vec2<T> operator-() const {
        return {
            -x,
            -y
        };
    }

    constexpr vec2<T> operator*(vec2<T> const& rhs) const {
        return {
            x * rhs.x,
            y * rhs.y
        };
    }

    constexpr vec2<T> operator/(vec2<T> const& rhs) const {
        return {
            x / rhs.x,
            y / rhs.y
        };
    }

    template<numeric U>
    constexpr vec2<T> operator+(U rhs) const {
        return {
            x + rhs,
            y + rhs
        };
    }

    template<numeric U>
    constexpr vec2<T> operator-(U rhs) const {
        return {
            x - rhs,
            y - rhs
        };
    }

    template<numeric U>
    constexpr vec2<T> operator*(U rhs) const {
        return {
            x * rhs,
            y * rhs
        };
    }

    template<numeric U>
    constexpr vec2<T> operator/(U rhs) const {
        return {
            x / rhs,
            y / rhs
        };
    }

    constexpr vec2<T>& operator+=(vec2<T> const& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator+=(U rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    constexpr vec2<T>& operator-=(vec2<T> const& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator-=(U rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    constexpr bool operator==(vec2<T> const& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    constexpr bool operator!=(vec2<T> const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator<=(vec2<T> const& rhs) const {
        return x <= rhs.x && y <= rhs.y;
    }

    constexpr bool operator>=(vec2<T> const& rhs) const {
        return x >= rhs.x && y >= rhs.y;
    }

    constexpr bool operator<(vec2<T> const& rhs) const {
        return x < rhs.x && y < rhs.y;
    }

    constexpr bool operator>(vec2<T> const& rhs) const {
        return x > rhs.x && y > rhs.y;
    }
};

struct mat3 {
    mat3() {
        m[0][0] = 1; m[0][1]; m[0][2];
        m[1][0]; m[1][1] = 1; m[1][2];
        m[2][0]; m[2][1]; m[2][2] = 1;
    }

    mat3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    ) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    static mat3 rotation(float radians);
    static mat3 skew(const vec2<float>& radians);
    static inline mat3 identity() {
        return {
            1.f, 0.f, 0,
            0.f, 1.f, 0,
            0.f, 0.f, 1
        };
    }

    static inline mat3 translation(const vec2<float>& t) {
        return {
            1.f, 0.f, t.x,
            0.f, 1.f, t.y,
            0.f, 0.f, 1.f
        };
    }

    static inline mat3 scale(const vec2<float>& s) {
        return {
            s.x, 0.f, 0,
            0.f, s.y, 0,
            0.f, 0.f, 1.f
        };
    }
    vec2<float> transform_point(const vec2<float>& p) const;
    inline vec2<float> translation() const {
        return { m[0][2], m[1][2] };
    }
    mat3 operator*(const mat3& o) const;

    float m[3][3];
};

} // namespace slv