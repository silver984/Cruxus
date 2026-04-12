#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/numeric.hpp>
#include <cstdint>
#include <algorithm>

namespace slv {

struct SLV_DLL rgb final {
    rgb();
    explicit rgb(uint8_t r_val, uint8_t g_val, uint8_t b_val);
    uint8_t r;
    uint8_t g;
    uint8_t b;

    [[nodiscard]] rgb operator+(rgb const& rhs) const;
    [[nodiscard]] rgb operator-(rgb const& rhs) const;
    [[nodiscard]] rgb operator-() const;
    [[nodiscard]] rgb operator*(rgb const& rhs) const;
    [[nodiscard]] rgb operator/(rgb const& rhs) const;
    rgb& operator+=(rgb const& rhs);
    rgb& operator-=(rgb const& rhs);
    rgb& operator*=(rgb const& rhs);
    rgb& operator/=(rgb const& rhs);
    bool operator==(rgb const& rhs) const;
    bool operator!=(rgb const& rhs) const;
    bool operator<=(rgb const& rhs) const;
    bool operator>=(rgb const& rhs) const;
    bool operator<(rgb const& rhs) const;
    bool operator>(rgb const& rhs) const;

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