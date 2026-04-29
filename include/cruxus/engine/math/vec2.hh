#pragma once
#include <cruxus/engine/math/numeric.hh>

namespace crx {

template<numeric T>
struct vec2 final {
    T x{};
    T y{};

    [[nodiscard]] constexpr vec2<T> operator +(vec2<T> const& rhs) const {
        return {
            .x = x + rhs.x,
            .y = y + rhs.y
        };
    }

    [[nodiscard]] constexpr vec2<T> operator -(vec2<T> const& rhs) const {
        return {
            .x = x - rhs.x,
            .y = y - rhs.y
        };
    }

    [[nodiscard]] constexpr vec2<T> operator -() const {
        return {
            .x = -x,
            .y = -y
        };
    }

    [[nodiscard]] constexpr vec2<T> operator *(vec2<T> const& rhs) const {
        return {
            .x = x * rhs.x,
            .y = y * rhs.y
        };
    }

    [[nodiscard]] constexpr vec2<T> operator /(vec2<T> const& rhs) const {
        return {
            .x = x / rhs.x,
            .y = y / rhs.y
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator +(U rhs) const {
        return {
            .x = x + rhs,
            .y = y + rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator -(U rhs) const {
        return {
            .x = x - rhs,
            .y = y - rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator *(U rhs) const {
        return {
            .x = x * rhs,
            .y = y * rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator /(U rhs) const {
        return {
            .x = x / rhs,
            .y = y / rhs
        };
    }

    constexpr vec2<T>& operator +=(vec2<T> const& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator +=(U rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    constexpr vec2<T>& operator -=(vec2<T> const& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator -=(U rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    constexpr vec2<T>& operator *=(vec2<T> const& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator /=(U rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    constexpr vec2<T>& operator ++() {
        ++x;
        ++y;
        return *this;
    }

    constexpr vec2<T> operator ++(int) {
        vec2<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr vec2<T>& operator --() {
        --x;
        --y;
        return *this;
    }

    constexpr vec2<T> operator --(int) {
        vec2<T> temp = *this;
        --(*this);
        return temp;
    }

    constexpr bool operator ==(vec2<T> const& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    constexpr bool operator !=(vec2<T> const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator <=(vec2<T> const& rhs) const {
        return x <= rhs.x && y <= rhs.y;
    }

    constexpr bool operator >=(vec2<T> const& rhs) const {
        return x >= rhs.x && y >= rhs.y;
    }

    constexpr bool operator <(vec2<T> const& rhs) const {
        return x < rhs.x && y < rhs.y;
    }

    constexpr bool operator >(vec2<T> const& rhs) const {
        return x > rhs.x && y > rhs.y;
    }
};

}