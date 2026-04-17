#pragma once
#include <slv/engine/math/numeric.hh>
#include <slv/engine/math/vec2.hh>
#include <slv/engine/math/size.hh>

namespace crx {

template<numeric T>
struct rect final {
	vec2<T> pos;
	size<T> bounds;
};

}