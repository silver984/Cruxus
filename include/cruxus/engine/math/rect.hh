#pragma once
#include <cruxus/engine/math/numeric.hh>
#include <cruxus/engine/math/vec2.hh>
#include <cruxus/engine/math/size.hh>

namespace crx {

template<numeric T>
struct rect final {
	vec2<T> pos;
	size<T> bounds;

	// TODO: operators
};

}