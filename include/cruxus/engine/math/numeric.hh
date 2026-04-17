#pragma once
#include <concepts>
#include <type_traits>

namespace crx {

template<typename T>
concept numeric = std::is_arithmetic_v<T>;

}