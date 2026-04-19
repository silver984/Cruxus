#pragma once
#include <crx/engine/math/numeric.hh>
#include <crx/engine/math/vec2.hh>
#include <crx/engine/math/size.hh>

namespace crx {

template<numeric T>
struct rect final {
	vec2<T> pos;
	size<T> bounds;

	// TODO: operators
};

}