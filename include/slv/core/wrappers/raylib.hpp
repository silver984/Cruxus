#pragma once

#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <slv/core/types/colors.hpp>

namespace slv::raylib
{
	void draw_rectangle(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha);
	void draw_rectangle_sharp_lines(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha, float thickness);
	void draw_texture(const slv::texture& texture, const slv::rect<float>& source, const slv::vec2<float>& offset, const slv::mat3& matrix, const slv::rgb& color, float alpha);
	void draw_line(const slv::mat3& matrix, const slv::vec2<float>& start_pos, const slv::vec2<float>& end_pos, const slv::rgb& color, float alpha, float thickness);
	void draw_circle(const slv::mat3& matrix, const slv::rgb& color, float alpha, float radius);
	void set_texture_antialiasing(const slv::texture& texture, bool val);

	/*
	slv::render_texture load_render_texture(int width, int height);
	void begin_texture_mode(const slv::render_texture& render_texture);
	void unload_render_texture(const slv::render_texture& render_texture);
	*/
}