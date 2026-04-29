#include "rl.hh"
#include <cruxus/engine/math/util.hh>
#ifdef _WIN32
#include "platforms/windows.hh"
#endif
#include <raylib.h>
#include <rlgl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <stb_image.h>
#include <cruxus/engine/debug/log.hh>
#include <fmt/format.h>

namespace {

Rectangle rl_rect(crx::vec2<float> const& p, crx::size<float> const& s) {
	return {
		.x = p.x,
		.y = p.y,
		.width = s.width,
		.height = s.height
	};
}

Color rl_color(crx::rgb const& color, float alpha) {
	return {
		.r = color.r,
		.g = color.g,
		.b = color.b,
		.a = static_cast<uint8_t>(255.f * alpha)
	};
}

Texture rl_texture(crx::texture const& texture) {
	return {
		.id = texture.id,
		.width = texture.bounds.width,
		.height = texture.bounds.height,
		.mipmaps = texture.mipmaps,
		.format = static_cast<int>(texture.format)
	};
}

Vector2 rl_vector(crx::vec2<float> const& vec2) {
	return {
		.x = vec2.x,
		.y = vec2.y
	};
}

Matrix rl_matrix(crx::mat3 const& matrix) {
	return {
		.m0 = matrix.m[0][0], .m4 = matrix.m[0][1], .m8 = 0.f, .m12 = matrix.m[0][2],
		.m1 = matrix.m[1][0], .m5 = matrix.m[1][1], .m9 = 0.f, .m13 = matrix.m[1][2],
		.m2 = 0.f, .m6 = 0.f, .m10 = 1.f, .m14 = 0.f,
		.m3 = 0.f, .m7 = 0.f, .m11 = 0.f, .m15 = 1.f
	};
}

void rl_push_mult_matrix(crx::mat3 const& matrix) {
	rlPushMatrix();
	Matrix rm = rl_matrix(matrix);
	rlMultMatrixf(&rm.m0);
}

void set_window_aspect_ratio(GLFWwindow* const& window, crx::size<int> const& bounds) {
	int gcd = std::gcd(bounds.width, bounds.height);
	int aspect_w = bounds.width / gcd;
	int aspect_h = bounds.height / gcd;

	if (aspect_w <= 0) {
		aspect_w = 1;
	}

	if (aspect_h <= 0) {
		aspect_h = 1;
	}

	glfwSetWindowAspectRatio(window, aspect_w, aspect_h);
}

}

namespace crx::rl {

bool init_window(size<int> const& bounds, int fps, char const* title) {
	InitWindow(bounds.width, bounds.height, title);

	if (!GetWindowHandle()) {
		log::error("Couldn't initialize | Window handle is nullptr");
		return false;
	}

	set_window_aspect_ratio(glfwGetCurrentContext(), bounds);
	SetTargetFPS(fps);
	SetExitKey(KEY_NULL);
	
	return true;
}

void set_window_size(size<int> const& bounds) {
	GLFWwindow* glfw_window = glfwGetCurrentContext();

	if (!glfw_window) {
		return;
	}

	SetWindowSize(bounds.width, bounds.height);
	set_window_aspect_ratio(glfwGetCurrentContext(), bounds);
}

void set_texture_antialiasing(texture const& texture, bool val) {
	SetTextureFilter(
		rl_texture(texture),
		val
		? TEXTURE_FILTER_BILINEAR
		: TEXTURE_FILTER_POINT
	);
}

std::optional<texture> load_texture_stb(char const* file_path) {
	size<int> bounds;
	int channels;
	stbi_uc* data = stbi_load(file_path, &bounds.width, &bounds.height, &channels, STBI_rgb_alpha);

	if (!data) {
		const char* reason = stbi_failure_reason();

		log::error(
			fmt::format(
				"stbi failed to load \"{}\" | what: {}",
				file_path,
				reason ? reason : "unknown error"
			)
		);

		return std::nullopt;
	} else {
		log::debug(
			fmt::format(
				"stbi loaded \"{}\" | {}x{} channels: {}",
				file_path,
				bounds.width,
				bounds.height,
				channels
			)
		);
	}

	Image img = {
		.data = data,
		.width = bounds.width,
		.height = bounds.height,
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
	};

	Texture tex = LoadTextureFromImage(img);
	stbi_image_free(data);

	if (tex.id == 0) {
		log::error(fmt::format("GPU upload failed for \"{}\"", file_path));
		return std::nullopt;
	}

	return texture(
		tex.id,
		size<int>(tex.width, tex.height),
		tex.mipmaps,
		static_cast<pixel_format>(tex.format)
	);
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
		rl_rect(source.pos, source.bounds),
		Rectangle(
			offsets.x,
			offsets.y,
			source.bounds.width,
			source.bounds.height
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


}