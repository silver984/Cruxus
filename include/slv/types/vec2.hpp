#pragma once
#include <slv/types/numeric_t.hpp>

namespace slv {

template<numeric_t T>
struct vec2 {
    constexpr vec2() = default;
    constexpr explicit vec2(T x_val, T y_val) :
        x(x_val),
        y(y_val)
    {}

    T x{};
    T y{};

    [[nodiscard]] constexpr vec2<T> operator+(vec2<T> const& rhs) const {
        return vec2<T>(
            x + rhs.x,
            y + rhs.y
        );
    }

    [[nodiscard]] constexpr vec2<T> operator-(vec2<T> const& rhs) const {
        return vec2<T>(
            x - rhs.x,
            y - rhs.y
        );
    }

    [[nodiscard]] constexpr vec2<T> operator-() const {
        return vec2<T>(
            -x,
            -y
        );
    }

    [[nodiscard]] constexpr vec2<T> operator*(vec2<T> const& rhs) const {
        return vec2<T>(
            x * rhs.x,
            y * rhs.y
        );
    }

    [[nodiscard]] constexpr vec2<T> operator/(vec2<T> const& rhs) const {
        return vec2<T>(
            x / rhs.x,
            y / rhs.y
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator+(U rhs) const {
        return vec2<T>(
            x + rhs,
            y + rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator-(U rhs) const {
        return vec2<T>(
            x - rhs,
            y - rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator*(U rhs) const {
        return vec2<T>(
            x * rhs,
            y * rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr vec2<T> operator/(U rhs) const {
        return vec2<T>(
            x / rhs,
            y / rhs
        );
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

    constexpr vec2<T>& operator*=(vec2<T> const& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T>& operator/=(U rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    constexpr vec2<T>& operator++() {
        ++x;
        ++y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T> operator++(int) {
        vec2<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr vec2<T>& operator--() {
        --x;
        --y;
        return *this;
    }

    template<numeric U>
    constexpr vec2<T> operator--(int) {
        vec2<T> temp = *this;
        --(*this);
        return temp;
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

}