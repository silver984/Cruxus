#include <slv/core/wrappers/raylib.hpp>
#include <raylib.h>
#include <rlgl.h>
#include <algorithm>

namespace
{
	Rectangle rl_rect(const slv::rect<float>& rect)
	{
		return { rect.x, rect.y, rect.width, rect.height };
	}

	Color rl_color(const slv::rgb& color, float alpha)
	{
		return { color.r, color.g, color.b, static_cast<unsigned char>(255.0F * alpha) };
	}

	Texture rl_texture(const slv::texture& texture)
	{
		return { texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
	}

	Vector2 rl_vector(const slv::vec2<float>& vec2)
	{
		return { vec2.x, vec2.y };
	}

	Matrix rl_matrix(const slv::mat3& matrix)
	{
		return Matrix(// column 0
					  matrix.m_[0][0],  // m0
					  matrix.m_[1][0],  // m1
					  0.0f,        // m2
					  0.0f,        // m3

					  // column 1
					  matrix.m_[0][1],  // m4
					  matrix.m_[1][1],  // m5
					  0.0f,        // m6
					  0.0f,        // m7

					  // column 2
					  0.0f,        // m8
					  0.0f,        // m9
					  1.0f,        // m10
					  0.0f,        // m11

					  // column 3 (translation)
					  matrix.m_[0][2],  // m12 (x)
					  matrix.m_[1][2],  // m13 (y)
					  0.0f,        // m14
					  1.0f         // m15
		);
	}

	/*
	RenderTexture2D rl_render_texture(const slv::render_texture& render_texture)
	{
		return { render_texture.id, rl_texture(render_texture.tex), rl_texture(render_texture.depth) };
	}
	*/

	// ----------------------------------------------------- //

	slv::texture slv_texture(const Texture& texture)
	{
		return { texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
	}

	void push_matrix(const slv::mat3& matrix)
	{
		rlPushMatrix();
		Matrix rm = rl_matrix(matrix);
		rlMultMatrixf((const float*)&rm);
	}
}

namespace slv::raylib
{
	void draw_rectangle(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha)
	{
		push_matrix(matrix);
		DrawRectanglePro(Rectangle(0.f, 0.f, size.width, size.height), Vector2(0.f, 0.f), 0.f, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_round_rectangle(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha, float roundness, int segments)
	{
		push_matrix(matrix);
		DrawRectangleRounded(Rectangle(0.f, 0.f, size.width, size.height), roundness, segments, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_rectangle_lines(const slv::mat3& matrix, const slv::vec2<float>& offset, const slv::size<float>& size, const slv::rgb& color, float alpha, float thickness)
	{
		push_matrix(matrix);
		DrawRectangleLinesEx(Rectangle(offset.x, offset.y, size.width, size.height), thickness, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_round_rectangle_lines(const slv::mat3& matrix, const slv::vec2<float>& offset, const slv::size<float>& size, const slv::rgb& color, float alpha, float thickness, float roundness, int segments)
	{
		push_matrix(matrix);
		DrawRectangleRoundedLinesEx(Rectangle(offset.x, offset.y, size.width, size.height), roundness, segments, thickness, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_texture(const slv::texture& texture, const slv::rect<float>& source, const slv::vec2<float>& offset, const slv::mat3& matrix, const slv::rgb& color, float alpha)
	{
		push_matrix(matrix);
		DrawTexturePro(rl_texture(texture), rl_rect(source), Rectangle(offset.x, offset.y, source.width, source.height), Vector2(0.f, 0.f), 0.f, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_line(const slv::mat3& matrix, const slv::vec2<float>& start_pos, const slv::vec2<float>& end_pos, const slv::rgb& color, float alpha, float thickness)
	{
		push_matrix(matrix);
		DrawLineEx(rl_vector(start_pos), rl_vector(end_pos), thickness, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_circle(const slv::mat3& matrix, const slv::rgb& color, float alpha, float radius)
	{
		push_matrix(matrix);
		DrawCircleV(Vector2(0.f, 0.f), radius, rl_color(color, alpha));
		rlPopMatrix();
	}

	void set_texture_antialiasing(const slv::texture& texture, bool val)
	{
		SetTextureFilter(rl_texture(texture), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}

	/*
	slv::render_texture load_render_texture(int width, int height)
	{
		RenderTexture2D render_texture_rl = LoadRenderTexture(std::max(width, 1), std::max(height, 1));
		return { render_texture_rl.id, slv_texture(render_texture_rl.texture), slv_texture(render_texture_rl.depth) };
	}

	void begin_texture_mode(const slv::render_texture& render_texture)
	{
		BeginTextureMode(rl_render_texture(render_texture));
	}

	void unload_render_texture(const slv::render_texture& render_texture)
	{
		UnloadRenderTexture(rl_render_texture(render_texture));
	}
	*/
}