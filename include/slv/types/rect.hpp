#pragma once
#include <slv/types/numeric_t.hpp>
#include <slv/types/vec2.hpp>
#include <slv/types/size.hpp>

namespace slv {

template<numeric_t T>
struct rect final {
	constexpr rect() = default;
	constexpr explicit rect(vec2<T> pos_val, size<T> dimensions_val) :
		pos(pos_val),
		bounds(dimensions_val)
	{}
	constexpr explicit rect(T x, T y, T width, T height) :
		pos(x, y),
		bounds(width, height)
	{}

	vec2<T> pos;
	size<T> bounds;
};

}