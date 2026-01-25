#pragma once

#include <cstdint>

namespace slv
{
    // 2D size with floating-point values
    struct size
    {
        float width{}; // Width
        float height{}; // Height

        size operator + (const size& other) const
        {
            return { width + other.width, height + other.height };
        }

        size operator - (const size& other) const
        {
            return { width - other.width, height - other.height };
        }

        size operator * (float scalar) const
        {
            return { width * scalar, height * scalar };
        }

        size operator / (float scalar) const
        {
            return { width / scalar, height / scalar };
        }

        size& operator += (const size& other)
        {
            width += other.width;
            height += other.height;
            return *this;
        }

        size& operator -= (const size& other)
        {
            width -= other.width;
            height -= other.height;
            return *this;
        }
    };

    // 2D size with integer values
    struct size_int
    {
        int width{}; // Width
        int height{}; // Height

        size_int operator + (const size_int& other) const
        {
            return { width + other.width, height + other.height };
        }

        size_int operator - (const size_int& other) const
        {
            return { width - other.width, height - other.height };
        }

        size_int operator * (int scalar) const
        {
            return { width * scalar, height * scalar };
        }

        size_int operator / (int scalar) const
        {
            return { width / scalar, height / scalar };
        }

        size_int& operator += (const size_int& other)
        {
            width += other.width;
            height += other.height;
            return *this;
        }

        size_int& operator -= (const size_int& other)
        {
            width -= other.width;
            height -= other.height;
            return *this;
        }
    };

    // 2D size with unsigned integer values
    struct size_uint
    {
        unsigned int width{}; // Width
        unsigned int height{}; // Height

        size_uint operator + (const size_uint& other) const
        {
            return { width + other.width, height + other.height };
        }

        size_uint operator - (const size_uint& other) const
        {
            return { width - other.width, height - other.height };
        }

        size_uint operator * (int scalar) const
        {
            return { width * scalar, height * scalar };
        }

        size_uint operator / (int scalar) const
        {
            return { width / scalar, height / scalar };
        }

        size_uint operator / (size_uint other) const
        {
            return { width / other.width, height / other.height };
        }

        size_uint& operator += (const size_uint& other)
        {
            width += other.width;
            height += other.height;
            return *this;
        }

        size_uint& operator -= (const size_uint& other)
        {
            width -= other.width;
            height -= other.height;
            return *this;
        }

        bool operator != (const size_uint& other)
        {
            if (width != other.width || height != other.height)
            {
                return false;
            }

            return true;
        }
    };

    // Rectangle with integer coordinates and dimensions
    struct rect_int
    {
        int x{}; // X position of top-left corner
        int y{}; // Y position of top-left corner
        int width{}; // Width of rectangle
        int height{}; // Height of rectangle

        /*
        rect_int operator + (const vec_2& offset) const
        {
            int offset_x = static_cast<int>(offset.x);
            int offset_y = static_cast<int>(offset.y);
            return { x + offset_x, y + offset_y, width, height };
        }

        rect_int operator - (const vec_2& offset) const
        {
            int offset_x = static_cast<int>(offset.x);
            int offset_y = static_cast<int>(offset.y);
            return { x - offset_x, y - offset_y, width, height };
        }

        rect_int& operator += (const vec_2& offset)
        {
            int offset_x = static_cast<int>(offset.x);
            int offset_y = static_cast<int>(offset.y);
            x += offset_x;
            y += offset_y;
            return *this;
        }

        rect_int& operator -= (const vec_2& offset)
        {
            int offset_x = static_cast<int>(offset.x);
            int offset_y = static_cast<int>(offset.y);
            x -= offset_x;
            y -= offset_y;
            return *this;
        }
        */
    };

    // Rectangle with floating-point coordinates and dimensions
    struct rect
    {
        float x{}; // X position of top-left corner
        float y{}; // Y position of top-left corner
        float width{}; // Width of rectangle
        float height{}; // Height of rectangle

        /*
        rect operator + (const vec_2& offset) const
        {
            return { x + offset.x, y + offset.y, width, height };
        }

        rect operator - (const vec_2& offset) const
        {
            return { x - offset.x, y - offset.y, width, height };
        }

        rect& operator += (const vec_2& offset)
        {
            x += offset.x; y += offset.y; return *this;
        }

        rect& operator -= (const vec_2& offset)
        {
            x -= offset.x; y -= offset.y; return *this;
        }
        */
    };

    // 2D vector
    struct vec_2
    {
        float x{}; // X component
        float y{}; // Y component

        vec_2 operator + (const vec_2& other) const
        {
            return vec_2{ x + other.x, y + other.y };
        }

        vec_2 operator - (const vec_2& other) const
        {
            return vec_2{ x - other.x, y - other.y };
        }

        vec_2 operator * (const vec_2& scalar) const
        {
            return vec_2{ x * scalar.x, y * scalar.y };
        }

        vec_2 operator * (float scalar) const
        {
            return vec_2{ x * scalar, y * scalar };
        }

        vec_2 operator / (const vec_2& scalar) const
        {
            return vec_2{ x / scalar.x, y / scalar.y };
        }

        vec_2& operator += (const vec_2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        vec_2& operator -= (const vec_2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
    };

    // 3D vector
    struct vec_3
    {
        float x{}; // X component
        float y{}; // Y component
        float z{}; // Z component
    };

    // 4D vector
    struct vec_4
    {
        float x{}; // X component
        float y{}; // Y component
        float z{}; // Z component
        float w{}; // W component
    };

    // Color with 8-bit components [0, 255]
    struct rgb_8
    {
        uint8_t r{}; // Red component
        uint8_t g{}; // Green component
        uint8_t b{}; // Blue component
    };

    // Simple numeric range
    struct range
    {
        float min{}; // Minimum value
        float max{}; // Maximum value
    };

    // Circle with center position and radius
    struct circle
    {
        vec_2 center{}; // Center position
        float radius{}; // Radius
    };

    // Simple transform
    struct transform
    {
        vec_2 pos{};
        vec_2 anchor{}; // Anchor point [0, 1]
        // Scale
        // (1, 1) on default
        vec_2 scale{ 1.f, 1.f };
        float rotation{}; // Rotation in degrees
        float alpha{ 1.f }; // Opacity [0, 1]
    };

    /*
    struct texture
    {

    };
    */
}