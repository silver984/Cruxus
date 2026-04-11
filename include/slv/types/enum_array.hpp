#pragma once
#include <array>
#include <cstddef>
#include <type_traits>

namespace slv {

template <typename T, typename enum_t, enum_t N>
requires std::is_enum_v<enum_t>
class enum_array {
public:
	T& operator[](enum_t e) {
		return data_[static_cast<size_t>(e)];
	}

	T const& operator[](enum_t e) const {
		return data_[static_cast<size_t>(e)];
	}

	T& at(enum_t e) {
		return data_.at(static_cast<size_t>(e));
	}

	T const& at(enum_t e) const {
		return data_.at(static_cast<size_t>(e));
	}

private:
	std::array<T, static_cast<size_t>(N)> data_;
};

}