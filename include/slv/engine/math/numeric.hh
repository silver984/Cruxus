#pragma once
#include <concepts>
#include <type_traits>

namespace slv {

template<typename T>
concept numeric = std::is_arithmetic_v<T>;

}