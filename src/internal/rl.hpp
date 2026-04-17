#pragma once
#include <slv/types/primitives.hpp>
#include <slv/types/graphic.hpp>
#include <optional>

namespace crx::rl {

bool init_window(size<int> const& bounds, int fps, char const* title);
void set_window_size(size<int> const& bounds);
void set_texture_antialiasing(texture const& texture, bool val);
std::optional<texture> load_texture_stb(char const* file_path);

void draw_rectangle(
	mat3 const& matrix,
	size<float> const& bounds,
	rgb const& color,
	float alpha
);

void draw_rectangle_lines(
	mat3 const& matrix,
	vec2<float> const& offset,
	size<float> const& bounds,
	rgb const& color,
	float alpha,
	float thickness
);

void draw_texture(
	texture const& texture,
	rect<float> const& source,
	vec2<float> const& offsets,
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

}