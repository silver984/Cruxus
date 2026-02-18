#pragma once

#include <slv/core/types/primitives.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace slv::math
{
    template <typename T>
    inline T lerp(T a, T b, T t)
    {
        return a + (b - a) * t;
    }

    template <typename T>
    inline int sign(T val)
    {
        T zero = static_cast<T>(0); // explicit
        return (val > zero) - (val < zero);
    }

    template <typename T>
    inline T avg(T a, T b)
    {
        T two = static_cast<T>(2); // explicit
        return (a + b) / two;
    }

    template <typename T>
    inline T map(T val, T in_min, T in_max, T out_min, T out_max)
    {
        return out_min + (out_max - out_min) * ((val - in_min) / (in_max - in_min));
    }

    template <typename T>
    inline slv::vec2<T> lerp(const slv::vec2<T>& a, const slv::vec2<T>& b, T t)
    {
        return slv::vec2<T>(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
    }

    template <typename T>
    inline slv::vec2<T> clamp(const slv::vec2<T>& val, const slv::vec2<T>& min_val, const slv::vec2<T>& max_val)
    {
        return slv::vec2<T>(std::clamp(val.x, min_val.x, max_val.x), std::clamp(val.y, min_val.y, max_val.y));
    }

    template <typename T>
    inline slv::vec2<T> normalize(const slv::vec2<T>& val)
    {
        T len = std::sqrt(val.x * val.x + val.y * val.y);
        T zero = static_cast<T>(0); // explicit
        return len == zero ? slv::vec2<T>(zero, zero) : slv::vec2<T>(val.x / len, val.y / len);
    }

    template <typename T>
    inline T length(const slv::vec2<T>& val)
    {
        return std::sqrt(val.x * val.x + val.y * val.y);
    }

    template <typename T>
    inline T distance(const slv::vec2<T>& a, const slv::vec2<T>& b)
    {
        return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }

    template <typename T>
    inline T dot(const slv::vec2<T>& a, const slv::vec2<T>& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    template <typename T>
    inline slv::vec2<T> damp(const slv::vec2<T>& current, const slv::vec2<T>& target, T lambda, T dt)
    {
        T one = static_cast<T>(1); // explicit
        T t = one - std::exp(-lambda * dt);
        return lerp(current, target, t);
    }

    template <typename T>
    inline T deg_to_rad(T deg)
    {
        T angle = static_cast<T>(180); // explicit
        return deg * (std::numbers::pi / angle);
    }

    template <typename T>
    inline T rad_to_deg(T rad)
    {
        T angle = static_cast<T>(180); // explicit
        return rad * (angle / std::numbers::pi);
    }

    template <typename T>
    inline slv::vec2<T> rotate(slv::vec2<T> point, slv::vec2<T> origin, T rotation_deg)
    {
        T rad = deg_to_rad(rotation_deg);
        slv::vec2<T> unrotated = slv::vec2<T>(point.x - origin.x, point.y - origin.y);
        slv::vec2<T> rotated = slv::vec2<T>(unrotated.x * std::cos(rad) - unrotated.y * std::sin(rad), unrotated.x * std::sin(rad) + unrotated.y * std::cos(rad));
        return slv::vec2<T>(rotated.x + origin.x, rotated.y + origin.y);
    }
}
