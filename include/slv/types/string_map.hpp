#pragma once
#include <unordered_map>
#include <string>
#include <string_view>
#include <cstddef>

namespace slv {

struct transparent_string_hash final {
    using is_transparent = void;

    size_t operator()(std::string_view s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }

    size_t operator()(std::string const& s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }

    size_t operator()(char const* s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};

template <typename T>
using string_map = std::unordered_map<std::string, T, transparent_string_hash, std::equal_to<>>;

}