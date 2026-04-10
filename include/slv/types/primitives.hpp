#pragma once
#include <slv/core/dll.hpp>
#include <cstdint>
#include <concepts>
#include <type_traits>
#include <algorithm>

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
    rgb& operator+=(rgb const& rhs);
    rgb& operator-=(rgb const& rhs);
    rgb& operator*=(rgb const& rhs);
    rgb& operator/=(rgb const& rhs);
    bool operator==(rgb const& rhs) const;
    bool operator!=(rgb const& rhs) const;
    bool operator<=(rgb const& rhs) const;
    bool operator>=(rgb const& rhs) const;
    bool operator<(rgb const& rhs) const;
    bool operator>(rgb const& rhs) const;

    template <numeric U>
    constexpr rgb operator+(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator-(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator*(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    constexpr rgb operator/(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template <numeric U>
    constexpr rgb& operator+=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator-=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator*=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator/=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
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

} // namespace slv