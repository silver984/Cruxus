#include <slv/internal/raylib.hpp>
#include <slv/engine/math.hpp>
#ifdef _WIN32
#include <platforms/windows.hpp>
#endif
#include <raylib.h>
#include <rlgl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>

namespace {

Rectangle rl_rect(slv::vec2<float> const& p, slv::size<float> const& s) {
	return {
		p.x,
		p.y,
		s.width,
		s.height
	};
}

Color rl_color(slv::rgb const& color, float alpha) {
	return {
		color.r,
		color.g,
		color.b,
		static_cast<uint8_t>(255.f * alpha)
	};
}

Texture rl_texture(slv::texture const& texture) {
	return {
		texture.id,
		texture.resolution.width,
		texture.resolution.height,
		texture.mipmaps,
		texture.format
	};
}

Vector2 rl_vector(slv::vec2<float> const& vec2) {
	return {
		vec2.x,
		vec2.y
	};
}

Matrix rl_matrix(slv::mat3 const& matrix) {
	return {
		matrix.m[0][0], matrix.m[1][0],
		0.f, 0.f,
		matrix.m[0][1], matrix.m[1][1],
		0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
		matrix.m[0][2], matrix.m[1][2],
		0.f, 1.f
	};
}

void rl_push_mult_matrix(slv::mat3 const& matrix) {
	rlPushMatrix();
	Matrix rm = rl_matrix(matrix);
	rlMultMatrixf(&rm.m0);
}

}

namespace slv::raylib {

bool init_window(size<int> const& dimensions, int fps, char const* title) {
	InitWindow(dimensions.width, dimensions.height, title);

	if (!IsWindowReady() || !GetWindowHandle()) {
		return false;
	}

	int monitor = GetCurrentMonitor();
	SetWindowPosition(
		(GetMonitorWidth(monitor) / 2) - (dimensions.width / 2),
		(GetMonitorHeight(monitor) / 2) - (dimensions.height / 2)
	);
	set_window_size(dimensions);
	SetTargetFPS(fps);
	SetExitKey(KEY_NULL);
	return true;
}

void set_window_size(size<int> const& dimensions) {
	GLFWwindow* glfw_window = glfwGetCurrentContext();

	if (!glfw_window) {
		return;
	}

	SetWindowSize(dimensions.width, dimensions.height);

	int gcd = std::gcd(dimensions.width, dimensions.height);
	int aspect_w = dimensions.width / gcd;
	int aspect_h = dimensions.height / gcd;

	if (aspect_w <= 0) {
		aspect_w = 1;
	}

	if (aspect_h <= 0) {
		aspect_h = 1;
	}

	glfwSetWindowAspectRatio(glfw_window, aspect_w, aspect_h);
}

void draw_rectangle(
	mat3 const& matrix,
	size<float> const& size,
	rgb const& color,
	float alpha
) {
	rl_push_mult_matrix(matrix);
	
	DrawRectanglePro(
		Rectangle(0.f, 0.f, size.width, size.height),
		Vector2(0.f, 0.f),
		0.f,
		rl_color(color, alpha)
	);
	
	rlPopMatrix();
}

void draw_rectangle_lines(
	mat3 const& matrix,
	vec2<float> const& offset,
	size<float> const& size,
	rgb const& color,
	float alpha,
	float thickness
) {
	rl_push_mult_matrix(matrix);

	DrawRectangleLinesEx(
		Rectangle(offset.x, offset.y, size.width, size.height),
		thickness,
		rl_color(color, alpha)
	);
	
	rlPopMatrix();
}

void draw_texture(
	texture const& texture,
	rect<float> const& source,
	vec2<float> const& offsets,
	mat3 const& matrix,
	rgb const& color,
	float alpha
) {
	rl_push_mult_matrix(matrix);
	
	DrawTexturePro(
		rl_texture(texture),
		rl_rect(source.pos, source.dimensions),
		Rectangle(
			offsets.x,
			offsets.y,
			source.dimensions.width,
			source.dimensions.height
		),
		Vector2(0.f, 0.f),
		0.f,
		rl_color(color, alpha)
	);
	
	rlPopMatrix();
}

void draw_line(
	mat3 const& matrix,
	vec2<float> const& start_pos,
	vec2<float> const& end_pos,
	rgb const& color,
	float alpha,
	float thickness
) {
	rl_push_mult_matrix(matrix);

	DrawLineEx(
		rl_vector(start_pos),
		rl_vector(end_pos),
		thickness,
		rl_color(color, alpha)
	);
	
	rlPopMatrix();
}

void draw_circle(
	mat3 const& matrix,
	rgb const& color,
	float alpha,
	float radius
) {
	rl_push_mult_matrix(matrix);
	
	DrawCircleV(
		Vector2(0.f, 0.f),
		radius,
		rl_color(color, alpha)
	);
	
	rlPopMatrix();
}

void set_texture_antialiasing(texture const& texture, bool val) {
	SetTextureFilter(
		rl_texture(texture), val
		? TEXTURE_FILTER_BILINEAR
		: TEXTURE_FILTER_POINT
	);
}

}