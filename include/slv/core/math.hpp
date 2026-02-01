#pragma once

#include <slv/core/types.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace slv::math
{
    /**
    * @brief Linearly interpolates between two floats.
    *
    * @param a Start value.
    * @param b End value.
    * @param t Interpolation factor [0, 1].
    * @return Interpolated value.
    */
    inline float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    /**
    * @brief Returns the sign of a float.
    *
    * @param v Value to check.
    * @return 1 if positive, -1 if negative, 0 if zero.
    */
    inline float sign(float v)
    {
        return (v > 0) - (v < 0);
    }

    /**
        * @brief Clamps a float between a minimum and maximum value.
        *
        * @param v Value to clamp.
        * @param min_val Minimum value.
        * @param max_val Maximum value.
        * @return Clamped value.
        */
    inline float clamp(float v, float min_val, float max_val)
    {
        return std::max(min_val, std::min(v, max_val));
    }

    /**
    * @brief Maps a value from one range to another.
    *
    * @param v Value to map.
    * @param in_min Minimum of input range.
    * @param in_max Maximum of input range.
    * @param out_min Minimum of output range.
    * @param out_max Maximum of output range.
    * @return Mapped value.
    */
    inline float map(float v, float in_min, float in_max, float out_min, float out_max)
    {
        return out_min + (out_max - out_min) * ((v - in_min) / (in_max - in_min));
    }

    /**
    * @brief Linearly interpolates between two 2D vectors.
    *
    * @param a Start vector.
    * @param b End vector.
    * @param t Interpolation factor [0, 1].
    * @return Interpolated vector.
    */
    template <typename T>
    inline slv::vec_2<T> lerp(const slv::vec_2<T>& a, const slv::vec_2<T>& b, float t)
    {
        return slv::vec_2{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }

    /**
    * @brief Clamps a 2D vector between min and max values for each component.
    *
    * @param v Vector to clamp.
    * @param min_x Minimum x value.
    * @param min_y Minimum y value.
    * @param max_x Maximum x value.
    * @param max_y Maximum y value.
    * @return Clamped vector.
    */
    template <typename T>
    inline slv::vec_2<T> clamp(const slv::vec_2<T>& v, float min_x, float min_y, float max_x, float max_y)
    {
        return slv::vec_2{ clamp(v.x, min_x, max_x), clamp(v.y, min_y, max_y) };
    }

    /**
    * @brief Normalizes a 2D vector (makes it length 1).
    *
    * @param v Vector to normalize.
    * @return Normalized vector, or (0, 0) if length is zero.
    */
    template <typename T>
    inline slv::vec_2<T> normalize(const slv::vec_2<T>& v)
    {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        return len == 0.f ? slv::vec_2<T>{ static_cast<T>(0), static_cast<T>(0) } : slv::vec_2<T>{ v.x / len, v.y / len };
    }

    /**
    * @brief Computes the length (magnitude) of a 2D vector.
    *
    * @param v Vector to measure.
    * @return Vector length.
    */
    template <typename T>
    inline T length(const slv::vec_2<T>& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    /**
    * @brief Computes the distance between two 2D points.
    *
    * @param a First point.
    * @param b Second point.
    * @return Distance between points.
    */
    template <typename T>
    inline T distance(const slv::vec_2<T>& a, const slv::vec_2<T>& b)
    {
        return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }

    /**
    * @brief Computes the dot product of two 2D vectors.
    *
    * @param a First vector.
    * @param b Second vector.
    * @return Dot product.
    */
    template <typename T>
    inline T dot(const slv::vec_2<T>& a, const slv::vec_2<T>& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /**
    * @brief Smoothly interpolates a vector towards a target using exponential damping.
    *
    * @param current Current vector.
    * @param target Target vector.
    * @param lambda Damping coefficient.
    * @param dt Delta time.
    * @return Damped vector.
    */
    template <typename T>
    inline slv::vec_2<T> damp(const slv::vec_2<T>& current, const slv::vec_2<T>& target, float lambda, float dt)
    {
        float t = 1.f - std::exp(-lambda * dt);
        return lerp(current, target, t);
    }

    /**
    * @brief Converts degrees to radians.
    *
    * @param deg Angle in degrees.
    * @return Angle in radians.
    */
    inline float deg_to_rad(float deg)
    {
        return deg * (std::numbers::pi / 180.f);
    }

    /**
    * @brief Converts radians to degrees.
    *
    * @param rad Angle in radians.
    * @return Angle in degrees.
    */
    inline float rad_to_deg(float rad)
    {
        return rad * (180.f / std::numbers::pi);
    }

    /**
    * @brief Rotates a point around a given origin.
    *
    * @param point Point to rotate.
    * @param origin Center of rotation.
    * @param rotation_deg Rotation angle in degrees.
    * @return Rotated point.
    */
    template <typename T>
    inline slv::vec_2<T> rotate(slv::vec_2<T> point, slv::vec_2<T> origin, float rotation_deg)
    {
        float rad = deg_to_rad(rotation_deg);

        slv::vec_2<T> unrotated
        {
            point.x - origin.x,
            point.y - origin.y
        };

        slv::vec_2<T> rotated
        {
            unrotated.x * cosf(rad) - unrotated.y * sinf(rad),
            unrotated.x * sinf(rad) + unrotated.y * cosf(rad)
        };

        return { rotated.x + origin.x, rotated.y + origin.y };
    }

        /*
        PROBABLY TEST THIS ONCE THE ENGINE IS WORKING
    
    inline slv::transform combine_transform(const slv::transform& world, const slv::transform& local)
    {
        slv::transform out;

        out.alpha = world.alpha * local.alpha;
        out.rotation = world.rotation + local.rotation;
        out.scale = world.scale * local.scale;
        out.pos = world.pos + local.pos;

        float rad = slv::util::math::deg_to_rad(world.rotation);
        float cos_r = cosf(rad);
        float sin_r = sinf(rad);

        vec_2 local = local.pos * world.scale;
        out.pos.x = cos_r * local.pos.x - sin_r * local.pos.y + world.pos.x;
        out.pos.y = sin_r * local.pos.x + cos_r * local.pos.y + world.pos.y;

        out.anchor = world.anchor + local.anchor * world.scale;


        return out;
    }
        */
}
