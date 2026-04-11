#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/numeric.hpp>
#include <slv/types/vec2.hpp>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace slv::math {

constexpr float pi = std::numbers::pi_v<float>;

template <numeric T>
T lerp(T a, T b, T t) {
    return a + (b - a) * t;
}

template <numeric T>
int sign(T val) {
    T zero = static_cast<T>(0);
    return (val > zero) - (val < zero);
}

template <numeric T>
T avg(T a, T b) {
    T two = static_cast<T>(2);
    return (a + b) / two;
}

template <numeric T>
T map(T val, T in_min, T in_max, T out_min, T out_max) {
    return out_min + (out_max - out_min) * ((val - in_min) / (in_max - in_min));
}

template <numeric T>
vec2<T> lerp(const vec2<T>& a, const vec2<T>& b, T t) {
    return vec2<T>(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

template <numeric T>
vec2<T> clamp(const vec2<T>& val, const vec2<T>& min_val, const vec2<T>& max_val) {
    return vec2<T>(std::clamp(val.x, min_val.x, max_val.x), std::clamp(val.y, min_val.y, max_val.y));
}

template <numeric T>
vec2<T> normalize(const vec2<T>& val) {
    T len = std::sqrt(val.x * val.x + val.y * val.y);
    T zero = static_cast<T>(0);
    return len == zero ? vec2<T>(zero, zero) : vec2<T>(val.x / len, val.y / len);
}

template <numeric T>
T length(const vec2<T>& val) {
    return std::sqrt(val.x * val.x + val.y * val.y);
}

template <numeric T>
T distance(const vec2<T>& a, const vec2<T>& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

template <numeric T>
T dot(const vec2<T>& a, const vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <numeric T>
vec2<T> damp(const vec2<T>& current, const vec2<T>& target, T lambda, T dt) {
    T one = static_cast<T>(1);
    T t = one - std::exp(-lambda * dt);
    return lerp(current, target, t);
}

SLV_DLL float deg2rad(float deg);
SLV_DLL float rad2deg(float rad);

template <numeric T>
vec2<T> rotate(vec2<T> point, vec2<T> origin, T rotation_deg) {
    T rad = deg2rad(rotation_deg);
    vec2<T> unrotated = vec2<T>(point.x - origin.x, point.y - origin.y);
    vec2<T> rotated = vec2<T>(
        unrotated.x * std::cos(rad) - unrotated.y * std::sin(rad),
        unrotated.x * std::sin(rad) + unrotated.y * std::cos(rad)
    );
    
    return vec2<T>(rotated.x + origin.x, rotated.y + origin.y);
}

}