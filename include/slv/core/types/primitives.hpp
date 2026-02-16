#pragma once

#include <cstdint>

namespace slv
{
    struct rgb
    {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
    };

    template<typename T>
    struct size
    {
        T width{};
        T height{};

        constexpr size<T> operator+(const size<T>& rhs) const
        {
            return { width + rhs.width, height + rhs.height };
        }

        constexpr size<T> operator-(const size<T>& rhs) const
        {
            return { width - rhs.width, height - rhs.height };
        }

        constexpr size<T> operator*(const size<T>& rhs) const
        {
            return { width * rhs.width, height * rhs.height };
        }

        constexpr size<T> operator/(const size<T>& rhs) const
        {
            return { width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr size<T> operator+(U rhs) const
        {
            return { width + rhs, height + rhs };
        }

        template<typename U>
        constexpr size<T> operator-(U rhs) const
        {
            return { width - rhs, height - rhs };
        }

        template<typename U>
        constexpr size<T> operator*(U rhs) const
        {
            return { width * rhs, height * rhs };
        }

        template<typename U>
        constexpr size<T> operator/(U rhs) const
        {
            return { width / rhs, height / rhs };
        }

        constexpr size<T>& operator+=(const size<T>& rhs)
        {
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size<T>& operator+=(U rhs)
        {
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr size<T>& operator-=(const size<T>& rhs)
        {
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size<T>& operator-=(U rhs)
        {
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const size<T>& rhs) const
        {
            return width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const size<T>& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const size<T>& rhs) const
        {
            return width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const size<T>& rhs) const
        {
            return width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const size<T>& rhs) const
        {
            return width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const size<T>& rhs) const
        {
            return width > rhs.width && height > rhs.height;
        }
    };

    template<typename T>
    struct rect
    {
        T x{};
        T y{};
        T width{};
        T height{};

        constexpr rect<T> operator+(const rect<T>& rhs) const
        {
            return { x + rhs.x, y + rhs.y, width + rhs.width, height + rhs.height };
        }

        constexpr rect<T> operator-(const rect<T>& rhs) const
        {
            return { x - rhs.x, y - rhs.y, width - rhs.width, height - rhs.height };
        }

        constexpr rect<T> operator*(const rect<T>& rhs) const
        {
            return { x * rhs.x, y * rhs.y, width * rhs.width, height * rhs.height };
        }

        constexpr rect<T> operator/(const rect<T>& rhs) const
        {
            return { x / rhs.x, y / rhs.y, width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr rect<T> operator+(U rhs) const
        {
            return { x + rhs, y + rhs, width + rhs, height + rhs };
        }

        template<typename U>
        constexpr rect<T> operator-(U rhs) const
        {
            return { x - rhs, y - rhs, width - rhs, height - rhs };
        }

        template<typename U>
        constexpr rect<T> operator*(U rhs) const
        {
            return { x * rhs, y * rhs, width * rhs, height * rhs };
        }

        template<typename U>
        constexpr rect<T> operator/(U rhs) const
        {
            return { x / rhs, y / rhs, width / rhs, height / rhs };
        }

        constexpr rect<T>& operator+=(const rect<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect<T>& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr rect<T>& operator-=(const rect<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect<T>& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const rect<T>& rhs) const
        {
            return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const rect<T>& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const rect<T>& rhs) const
        {
            return x <= rhs.x && y <= rhs.y && width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const rect<T>& rhs) const
        {
            return x >= rhs.x && y >= rhs.y && width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const rect<T>& rhs) const
        {
            return x < rhs.x && y < rhs.y && width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const rect<T>& rhs) const
        {
            return x > rhs.x && y > rhs.y && width > rhs.width && height > rhs.height;
        }
    };

    template<typename T>
    struct vec2
    {
        T x{};
        T y{};

        constexpr vec2<T> operator+(const vec2<T>& rhs) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        constexpr vec2<T> operator-(const vec2<T>& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        constexpr vec2<T> operator*(const vec2<T>& rhs) const
        {
            return { x * rhs.x, y * rhs.y };
        }

        constexpr vec2<T> operator/(const vec2<T>& rhs) const
        {
            return { x / rhs.x, y / rhs.y };
        }

        template<typename U>
        constexpr vec2<T> operator+(U rhs) const
        {
            return { x + rhs, y + rhs };
        }

        template<typename U>
        constexpr vec2<T> operator-(U rhs) const
        {
            return { x - rhs, y - rhs };
        }

        template<typename U>
        constexpr vec2<T> operator*(U rhs) const
        {
            return { x * rhs, y * rhs };
        }

        template<typename U>
        constexpr vec2<T> operator/(U rhs) const
        {
            return { x / rhs, y / rhs };
        }

        constexpr vec2<T>& operator+=(const vec2<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec2<T>& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            return *this;
        }

        constexpr vec2<T>& operator-=(const vec2<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec2<T>& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        constexpr bool operator==(const vec2<T>& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        constexpr bool operator!=(const vec2<T>& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const vec2<T>& rhs) const
        {
            return x <= rhs.x && y <= rhs.y;
        }

        constexpr bool operator>=(const vec2<T>& rhs) const
        {
            return x >= rhs.x && y >= rhs.y;
        }

        constexpr bool operator<(const vec2<T>& rhs) const
        {
            return x < rhs.x && y < rhs.y;
        }

        constexpr bool operator>(const vec2<T>& rhs) const
        {
            return x > rhs.x && y > rhs.y;
        }
    };
}