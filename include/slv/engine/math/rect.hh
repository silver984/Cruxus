#pragma once
#include <slv/engine/math/numeric.hh>
#include <slv/engine/math/vec2.hh>
#include <slv/engine/math/size.hh>

namespace slv {

template<numeric T>
struct rect final {
	vec2<T> pos;
	size<T> bounds;
};

}