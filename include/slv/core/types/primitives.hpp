#pragma once

namespace slv
{
    struct rgb
    {
        unsigned char r{};
        unsigned char g{};
        unsigned char b{};
    };

    template<typename T>
    struct size
    {
        T width{};
        T height{};

        constexpr size operator+(const size& rhs) const
        {
            return { width + rhs.width, height + rhs.height };
        }

        constexpr size operator-(const size& rhs) const
        {
            return { width - rhs.width, height - rhs.height };
        }

        constexpr size operator*(const size& rhs) const
        {
            return { width * rhs.width, height * rhs.height };
        }

        constexpr size operator/(const size& rhs) const
        {
            return { width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr size operator+(U rhs) const
        {
            return { width + rhs, height + rhs };
        }

        template<typename U>
        constexpr size operator-(U rhs) const
        {
            return { width - rhs, height - rhs };
        }

        template<typename U>
        constexpr size operator*(U rhs) const
        {
            return { width * rhs, height * rhs };
        }

        template<typename U>
        constexpr size operator/(U rhs) const
        {
            return { width / rhs, height / rhs };
        }

        constexpr size& operator+=(const size& rhs)
        {
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size& operator+=(U rhs)
        {
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr size& operator-=(const size& rhs)
        {
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr size& operator-=(U rhs)
        {
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const size& rhs) const
        {
            return width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const size& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const size& rhs) const
        {
            return width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const size& rhs) const
        {
            return width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const size& rhs) const
        {
            return width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const size& rhs) const
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

        constexpr rect operator+(const rect& rhs) const
        {
            return { x + rhs.x, y + rhs.y, width + rhs.width, height + rhs.height };
        }

        constexpr rect operator-(const rect& rhs) const
        {
            return { x - rhs.x, y - rhs.y, width - rhs.width, height - rhs.height };
        }

        constexpr rect operator*(const rect& rhs) const
        {
            return { x * rhs.x, y * rhs.y, width * rhs.width, height * rhs.height };
        }

        constexpr rect operator/(const rect& rhs) const
        {
            return { x / rhs.x, y / rhs.y, width / rhs.width, height / rhs.height };
        }

        template<typename U>
        constexpr rect operator+(U rhs) const
        {
            return { x + rhs, y + rhs, width + rhs, height + rhs };
        }

        template<typename U>
        constexpr rect operator-(U rhs) const
        {
            return { x - rhs, y - rhs, width - rhs, height - rhs };
        }

        template<typename U>
        constexpr rect operator*(U rhs) const
        {
            return { x * rhs, y * rhs, width * rhs, height * rhs };
        }

        template<typename U>
        constexpr rect operator/(U rhs) const
        {
            return { x / rhs, y / rhs, width / rhs, height / rhs };
        }

        constexpr rect& operator+=(const rect& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            width += rhs;
            height += rhs;
            return *this;
        }

        constexpr rect& operator-=(const rect& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            width -= rhs.width;
            height -= rhs.height;
            return *this;
        }

        template<typename U>
        constexpr rect& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            width -= rhs;
            height -= rhs;
            return *this;
        }

        constexpr bool operator==(const rect& rhs) const
        {
            return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
        }

        constexpr bool operator!=(const rect& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const rect& rhs) const
        {
            return x <= rhs.x && y <= rhs.y && width <= rhs.width && height <= rhs.height;
        }

        constexpr bool operator>=(const rect& rhs) const
        {
            return x >= rhs.x && y >= rhs.y && width >= rhs.width && height >= rhs.height;
        }

        constexpr bool operator<(const rect& rhs) const
        {
            return x < rhs.x && y < rhs.y && width < rhs.width && height < rhs.height;
        }

        constexpr bool operator>(const rect& rhs) const
        {
            return x > rhs.x && y > rhs.y && width > rhs.width && height > rhs.height;
        }
    };

    template<typename T>
    struct vec_2
    {
        T x{};
        T y{};

        constexpr vec_2 operator+(const vec_2& rhs) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        constexpr vec_2 operator-(const vec_2& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        constexpr vec_2 operator*(const vec_2& rhs) const
        {
            return { x * rhs.x, y * rhs.y };
        }

        constexpr vec_2 operator/(const vec_2& rhs) const
        {
            return { x / rhs.x, y / rhs.y };
        }

        template<typename U>
        constexpr vec_2 operator+(U rhs) const
        {
            return { x + rhs, y + rhs };
        }

        template<typename U>
        constexpr vec_2 operator-(U rhs) const
        {
            return { x - rhs, y - rhs };
        }

        template<typename U>
        constexpr vec_2 operator*(U rhs) const
        {
            return { x * rhs, y * rhs };
        }

        template<typename U>
        constexpr vec_2 operator/(U rhs) const
        {
            return { x / rhs, y / rhs };
        }

        constexpr vec_2& operator+=(const vec_2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec_2& operator+=(U rhs)
        {
            x += rhs;
            y += rhs;
            return *this;
        }

        constexpr vec_2& operator-=(const vec_2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        template<typename U>
        constexpr vec_2& operator-=(U rhs)
        {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        constexpr bool operator==(const vec_2& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        constexpr bool operator!=(const vec_2& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<=(const vec_2& rhs) const
        {
            return x <= rhs.x && y <= rhs.y;
        }

        constexpr bool operator>=(const vec_2& rhs) const
        {
            return x >= rhs.x && y >= rhs.y;
        }

        constexpr bool operator<(const vec_2& rhs) const
        {
            return x < rhs.x && y < rhs.y;
        }

        constexpr bool operator>(const vec_2& rhs) const
        {
            return x > rhs.x && y > rhs.y;
        }
    };
}