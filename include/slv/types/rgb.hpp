#pragma once
#include <slv/config.hpp>
#include <slv/types/numeric_t.hpp>
#include <cstdint>
#include <algorithm>

namespace slv {

struct SLV_DLL rgb final {
    constexpr rgb() :
        r(0),
        g(0),
        b(0)
    {}
    constexpr explicit rgb(uint8_t r_val, uint8_t g_val, uint8_t b_val) :
        r(r_val),
        g(g_val),
        b(b_val)
    {}

    uint8_t r;
    uint8_t g;
    uint8_t b;

    [[nodiscard]] constexpr rgb operator+(rgb const& rhs) const {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return rgb(
            evaluate(r, rhs.r),
            evaluate(g, rhs.g),
            evaluate(b, rhs.b)
        );
    }
    
    [[nodiscard]] constexpr rgb operator-(rgb const& rhs) const {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return rgb(
            evaluate(r, rhs.r),
            evaluate(g, rhs.g),
            evaluate(b, rhs.b)
        );
    }

    [[nodiscard]] constexpr rgb operator-() const {
        return rgb(
        static_cast<uint8_t>(255 - r),
        static_cast<uint8_t>(255 - g),
        static_cast<uint8_t>(255 - b)
        );
    }

    [[nodiscard]] constexpr rgb operator*(rgb const& rhs) const {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return rgb(
            evaluate(r, rhs.r),
            evaluate(g, rhs.g),
            evaluate(b, rhs.b)
        );
    }

    [[nodiscard]] constexpr rgb operator/(rgb const& rhs) const {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return rgb(
            evaluate(r, rhs.r),
            evaluate(g, rhs.g),
            evaluate(b, rhs.b)
        );
    }

    constexpr rgb& operator+=(rgb const& rhs) {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs.r);
        g = evaluate(g, rhs.g);
        b = evaluate(b, rhs.b);
        return *this;
    }

    constexpr rgb& operator-=(rgb const& rhs) {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs.r);
        g = evaluate(g, rhs.g);
        b = evaluate(b, rhs.b);
        return *this;
    }

    constexpr rgb& operator*=(rgb const& rhs) {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs.r);
        g = evaluate(g, rhs.g);
        b = evaluate(b, rhs.b);
        return *this;
    }

    constexpr rgb& operator/=(rgb const& rhs) {
        auto evaluate = [](uint8_t c, uint8_t v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs.r);
        g = evaluate(g, rhs.g);
        b = evaluate(b, rhs.b);
        return *this;
    }

    constexpr rgb& operator++() {
        if (r < 255) {
            ++r;
        }

        if (g < 255) {
            ++g;
        }

        if (b < 255) {
            ++b;
        }

        return *this;
    }

    constexpr rgb operator++(int) {
        rgb temp = *this;
        ++(*this);
        return temp;
    }

    constexpr rgb& operator--() {
        if (r > 0) {
            --r;
        }

        if (g > 0) {
            --g;
        }

        if (b > 0) {
            --b;
        }

        return *this;
    }

    constexpr rgb operator--(int) {
        rgb temp = *this;
        --(*this);
        return temp;
    }

    constexpr bool operator==(rgb const& rhs) const {
        return r == rhs.r && g == rhs.g && b == rhs.b;
    }

    constexpr bool operator!=(rgb const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator<=(rgb const& rhs) const {
        return r <= rhs.r && g <= rhs.g && b <= rhs.b;
    }

    constexpr bool operator>=(rgb const& rhs) const {
        return r >= rhs.r && g >= rhs.g && b >= rhs.b;
    }

    constexpr bool operator<(rgb const& rhs) const {
        return r < rhs.r && g < rhs.g && b < rhs.b;
    }

    constexpr bool operator>(rgb const& rhs) const {
        return r > rhs.r && g > rhs.g && b > rhs.b;
    }

    template <numeric U>
    [[nodiscard]] constexpr rgb operator+(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr rgb operator-(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr rgb operator*(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template<numeric U>
    [[nodiscard]] constexpr rgb operator/(U rhs) const {
        auto evaluate = [](uint8_t c, U v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        return {
            evaluate(r, rhs),
            evaluate(g, rhs),
            evaluate(b, rhs)
        };
    }

    template <numeric U>
    constexpr rgb& operator+=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c + v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator-=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c - v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator*=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c * v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }

    template<numeric U>
    constexpr rgb& operator/=(U rhs) {
        auto evaluate = [](uint8_t c, U v) {
            int result = c / v;
            return static_cast<uint8_t>(std::clamp(result, 0, 255));
            };

        r = evaluate(r, rhs);
        g = evaluate(g, rhs);
        b = evaluate(b, rhs);
        return *this;
    }
};

}