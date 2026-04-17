#pragma once
#include <slv/engine/math/numeric.hh>

namespace crx {

template<numeric T>
struct size final {
    T width{};
    T height{};

    [[nodiscard]] constexpr size<T> operator +(size<T> const& rhs) const {
        return {
            .width = width + rhs.width,
            .height = height + rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator -(size<T> const& rhs) const {
        return {
            .width = width - rhs.width,
            .height = height - rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator -() const {
        return {
            .width = -width,
            .height = -height
        };
    }

    [[nodiscard]] constexpr size<T> operator *(size<T> const& rhs) const {
        return {
            .width = width * rhs.width,
            .height = height * rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator /(size<T> const& rhs) const {
        return {
            .width = width / rhs.width,
            .height = height / rhs.height
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator +(U rhs) const {
        return {
            .width = width + rhs,
            .height = height + rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator -(U rhs) const {
        return {
            .width = width - rhs,
            .height = height - rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator *(U rhs) const {
        return {
            .width = width * rhs,
            .height = height * rhs
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator /(U rhs) const {
        return {
            .width = width / rhs,
            .height = height / rhs
        };
    }

    constexpr size<T>& operator +=(size<T> const& rhs) {
        width += rhs.width;
        height += rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator +=(U rhs) {
        width += rhs;
        height += rhs;
        return *this;
    }

    constexpr size<T>& operator -=(size<T> const& rhs) {
        width -= rhs.width;
        height -= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator -=(U rhs) {
        width -= rhs;
        height -= rhs;
        return *this;
    }

    constexpr size<T>& operator *=(size<T> const& rhs) {
        width *= rhs.width;
        height *= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator *=(U rhs) {
        width *= rhs;
        height *= rhs;
        return *this;
    }

    constexpr size<T>& operator /=(size<T> const& rhs) {
        width /= rhs.width;
        height /= rhs.height;
        return *this;
    }

    template<numeric U>
    constexpr size<T>& operator /=(U rhs) {
        width /= rhs;
        height /= rhs;
        return *this;
    }

    constexpr size<T>& operator ++() {
        ++width;
        ++height;
        return *this;
    }

    constexpr size<T> operator ++(int) {
        size<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr size<T>& operator --() {
        --width;
        --height;
        return *this;
    }

    constexpr size<T> operator --(int) {
        size<T> temp = *this;
        --(*this);
        return temp;
    }

    constexpr bool operator ==(size<T> const& rhs) const {
        return width == rhs.width && height == rhs.height;
    }

    constexpr bool operator !=(size<T> const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator <=(size<T> const& rhs) const {
        return width <= rhs.width && height <= rhs.height;
    }

    constexpr bool operator >=(size<T> const& rhs) const {
        return width >= rhs.width && height >= rhs.height;
    }

    constexpr bool operator <(size<T> const& rhs) const {
        return width < rhs.width && height < rhs.height;
    }

    constexpr bool operator >(size<T> const& rhs) const {
        return width > rhs.width && height > rhs.height;
    }
};

}