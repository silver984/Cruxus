#pragma once
#include <slv/types/primitives.hpp>
#include <slv/types/texture.hpp>
#include <slv/types/colors.hpp>

namespace slv::raylib {

bool init_window(int width, int height, int fps, char const* title);

void draw_rectangle(
	mat3 const& matrix,
	size<float> const& size,
	rgb const& color,
	float alpha
);

void draw_rectangle_lines(
	mat3 const& matrix,
	vec2<float> const& offset,
	size<float> const& size,
	rgb const& color,
	float alpha,
	float thickness
);

void draw_texture(
	texture const& texture,
	vec2<float> const& source_pos,
	size<float> const& source_size,
	vec2<float> const& offset,
	mat3 const& matrix,
	rgb const& color,
	float alpha
);

void draw_line(
	mat3 const& matrix,
	vec2<float> const& start_pos,
	vec2<float> const& end_pos,
	rgb const& color,
	float alpha,
	float thickness
);

void draw_circle(
	mat3 const& matrix,
	rgb const& color,
	float alpha,
	float radius
);

void set_texture_antialiasing(texture const& texture, bool val);

}