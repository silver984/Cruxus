#pragma once
#include <slv/types/numeric_t.hpp>

namespace slv {

template<numeric_t T>
struct size final {
    constexpr size() = default;
    constexpr explicit size(T width_val, T height_val) :
        width(width_val),
        height(height_val)
    {}

    T width{};
    T height{};

    [[nodiscard]] constexpr size<T> operator+(size<T> const& rhs) const {
        return size<T>(
            width + rhs.width,
            height + rhs.height
        );
    }

    [[nodiscard]] constexpr size<T> operator-(size<T> const& rhs) const {
        return size<T>(
            width - rhs.width,
            height - rhs.height
        );
    }

    [[nodiscard]] constexpr size<T> operator-() const {
        return size<T>(
            -width,
            -height
        );
    }

    [[nodiscard]] constexpr size<T> operator*(size<T> const& rhs) const {
        return size<T>(
            width * rhs.width,
            height * rhs.height
        );
    }

    [[nodiscard]] constexpr size<T> operator/(size<T> const& rhs) const {
        return size<T>(
            width / rhs.width,
            height / rhs.height
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator+(U rhs) const {
        return size<T>(
            width + rhs,
            height + rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator-(U rhs) const {
        return size<T>(
            width - rhs,
            height - rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator*(U rhs) const {
        return size<T>(
            width * rhs,
            height * rhs
        );
    }

    template<numeric U>
    [[nodiscard]] constexpr size<T> operator/(U rhs) const {
        return size<T>(
            width / rhs,
            height / rhs
        );
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

    constexpr size<T>& operator++() {
        ++width;
        ++height;
        return *this;
    }

    template<numeric U>
    constexpr size<T> operator++(int) {
        size<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr size<T>& operator--() {
        --width;
        --height;
        return *this;
    }

    template<numeric U>
    constexpr size<T> operator--(int) {
        size<T> temp = *this;
        --(*this);
        return temp;
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