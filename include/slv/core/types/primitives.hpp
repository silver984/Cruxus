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

        constexpr vec2<T> operator-() const
        {
            return { -x, -y };
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

    class mat3
    {
    public:
        mat3()
        {
            m_[0][0] = 1; m_[0][1] = 0; m_[0][2] = 0;
            m_[1][0] = 0; m_[1][1] = 1; m_[1][2] = 0;
            m_[2][0] = 0; m_[2][1] = 0; m_[2][2] = 1;
        }
        
        mat3(float m00, float m01, float m02,
             float m10, float m11, float m12,
             float m20, float m21, float m22)
        {
            m_[0][0] = m00; m_[0][1] = m01; m_[0][2] = m02;
            m_[1][0] = m10; m_[1][1] = m11; m_[1][2] = m12;
            m_[2][0] = m20; m_[2][1] = m21; m_[2][2] = m22;
        }

        static mat3 rotation(float radians);
        mat3 operator*(const mat3& o) const;
        vec2<float> transform_point(const vec2<float>& p) const;

        static inline mat3 identity()
        {
            return mat3(1, 0, 0,
                        0, 1, 0,
                        0, 0, 1);
        }

        static inline mat3 translation(const vec2<float>& t)
        {
            return mat3(1, 0, t.x,
                        0, 1, t.y,
                        0, 0, 1);
        }

        static inline mat3 scale(const vec2<float>& s)
        {
            return mat3(s.x, 0, 0,
                        0, s.y, 0,
                        0, 0, 1);
        }

        inline vec2<float> translation() const
        {
            return vec2<float>(m_[0][2], m_[1][2]);
        }

        float m_[3][3];
    };
}