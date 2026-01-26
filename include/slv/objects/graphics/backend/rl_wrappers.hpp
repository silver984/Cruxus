#pragma once

#include <slv/types.hpp>
#include <slv/colors.hpp>

namespace slv::raylib
{
	void draw_rectangle(const slv::rect& rect, const slv::vec_2& anchor, const slv::rgb_8& color, float alpha, float rotation);
	void draw_texture(const slv::texture& texture, const slv::rect& source, const slv::rect& dest, const slv::vec_2& anchor, float rotation, float alpha, const slv::rgb_8& color = slv::WHITE);
	void set_texture_antialiasing(const slv::texture& texture, bool toggle);
}