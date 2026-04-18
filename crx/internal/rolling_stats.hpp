#pragma once
#include <crx/engine/math/numeric.hh>
#include <array>
#include <type_traits>
#include <cstdint>

namespace crx {

template <numeric T, size_t N = 60>
struct rolling_stats {
private:
    using acc_t = std::conditional_t<std::is_floating_point_v<T>, double, int64_t>;

public:
    constexpr rolling_stats() = default;

    void push(T value) {
        last = value;

        if (count < N) {
            samples[index] = value;
            sum += static_cast<acc_t>(value);
            ++count;
        } else {
            const acc_t old = static_cast<acc_t>(samples[index]);
            samples[index] = value;
            sum += static_cast<acc_t>(value) - old;
        }

        if (++index == N) {
            index = 0;
        }

        if (count > 0) {
            avg = static_cast<T>(sum / static_cast<acc_t>(count));
        }
    }

    std::array<T, N> samples{};
    size_t index = 0;
    size_t count = 0;
    acc_t sum{};
    T avg{};
    T last{};
};

}