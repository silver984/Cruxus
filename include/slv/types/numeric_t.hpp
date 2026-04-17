#pragma once
#include <concepts>
#include <type_traits>

namespace slv {

template<typename T>
concept numeric_t = std::integral<T> || std::floating_point<T>;

}