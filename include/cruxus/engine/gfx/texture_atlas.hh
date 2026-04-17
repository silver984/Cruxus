#pragma once
#include <slv/config.hh>
#include <slv/engine/gfx/texture.hh>
#include <slv/engine/util/pointers.hh>
#include <slv/engine/math/rect.hh>
#include <slv/engine/math/vec2.hh>
#include <slv/engine/util/string_map.hh>
#include <vector>

namespace crx {
struct SLV_DLL texture_atlas final {
	struct element final {
		rect<int> rect; // pixel bounds in atlas
		vec2<float> offsets; // draw-time positional adjustment
	};

	string_map<element> elements;
	texture base_texture;
};
}