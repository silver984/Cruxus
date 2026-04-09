#pragma once
#include <slv/core/types/primitives.hpp>

namespace slv::color {
constexpr inline rgb red{ 255, 0, 0 };
constexpr inline rgb green{ 0, 255, 0 };
constexpr inline rgb blue{ 0, 0, 255 };
constexpr inline rgb yellow{ 255, 255, 0 };
constexpr inline rgb cyan{ 0, 255, 255 };
constexpr inline rgb magenta{ 255, 0, 255 };
constexpr inline rgb blank{ 0, 0, 0 };
constexpr inline rgb white{ 255, 255, 255 };
constexpr inline rgb gray{ 128, 128, 128 };
constexpr inline rgb orange{ 255, 165, 0 };
constexpr inline rgb pink{ 255, 192, 203 };
constexpr inline rgb purple{ 128, 0, 128 };
constexpr inline rgb brown{ 165, 42, 42 };
} // namespace slv::color