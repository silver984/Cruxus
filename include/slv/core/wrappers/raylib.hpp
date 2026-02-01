#pragma once

#include <slv/core/types.hpp>
#include <slv/core/colors.hpp>

namespace slv::raylib
{
	void draw_rectangle(const slv::rect<float>& rect, const slv::vec_2<float>& anchor, const slv::rgb& color, float alpha, float rotation);
	void draw_texture(const slv::texture& texture, const slv::rect<float>& source, const slv::rect<float>& dest, const slv::vec_2<float>& anchor, float rotation, float alpha, const slv::rgb& color = slv::colors::WHITE);
	void set_texture_antialiasing(const slv::texture& texture, bool toggle);
	slv::render_texture load_render_texture(unsigned int width, unsigned int height);
	void begin_texture_mode(const slv::render_texture& render_texture);
	void unload_render_texture(const slv::render_texture& render_texture);
}