#pragma once
#include <slv/types/numeric.hpp>

namespace slv {

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

}