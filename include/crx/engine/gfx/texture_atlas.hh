#pragma once
#include <crx/config.hh>
#include <crx/engine/gfx/texture.hh>
#include <crx/engine/math/rect.hh>
#include <crx/engine/math/vec2.hh>
#include <crx/engine/util/string_map.hh>
#include <vector>

namespace crx {

struct CRX_DLL texture_atlas final {
	struct CRX_DLL element final {
		rect<int> rect; // pixel bounds in atlas
		vec2<float> offsets; // draw-time positional adjustment
	};

	string_map<element> elements;
	texture base_texture;
};

}