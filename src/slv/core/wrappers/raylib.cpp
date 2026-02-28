#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/math.hpp>
#include <raylib.h>
#include <rlgl.h>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace
{
	Rectangle rl_rect(const slv::rect<float>& rect)
	{
		return { rect.x, rect.y, rect.width, rect.height };
	}

	Color rl_color(const slv::rgb& color, float alpha)
	{
		return { color.r, color.g, color.b, static_cast<unsigned char>(255.f * alpha) };
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
		return
		{
			matrix.m_[0][0], matrix.m_[1][0],
			0.0f, 0.0f,
			matrix.m_[0][1], matrix.m_[1][1],
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			matrix.m_[0][2], matrix.m_[1][2],
			0.0f, 1.0f
		};
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

	void rl_push_mult_matrix(const slv::mat3& matrix)
	{
		rlPushMatrix();
		Matrix rm = rl_matrix(matrix);
		rlMultMatrixf((const float*)&rm);
	}
}

namespace slv::raylib
{
	bool init_window(int width, int height, int fps, const char* title)
	{
		InitWindow(width, height, title);

		if (!IsWindowReady() || !GetWindowHandle())
		{
			return false;
		}

		SetTargetFPS(fps);
		SetExitKey(KEY_NULL);

		return true;
	}

	void draw_rectangle(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha)
	{
		rl_push_mult_matrix(matrix);
		DrawRectanglePro(Rectangle(0.f, 0.f, size.width, size.height), Vector2(0.f, 0.f), 0.f, rl_color(color, alpha));
		rlPopMatrix();
	}

	/*
	void draw_round_rectangle(const slv::mat3& matrix, const slv::size<float>& size, const slv::rgb& color, float alpha, float roundness, int segments)
	{
		if (size.width <= 0.f || size.height <= 0.f)
		{
			return;
		}

		segments = std::max(1, segments);
		float max_radius = std::min(size.width, size.height) * 0.5f;
		float radius = std::clamp(roundness, 0.f, 1.f) * max_radius;

		float left = 0.f;
		float right = size.width;
		float top = 0.f;
		float bottom = size.height;

		float inner_left = left + radius;
		float inner_right = right - radius;
		float inner_top = top + radius;
		float inner_bottom = bottom - radius;

		Color col = rl_color(color, alpha);

		rl_push_mult_matrix(matrix);
		rlColor4ub(col.r, col.g, col.b, col.a);
		rlBegin(RL_TRIANGLES);

		// ----- Center quad
		rlVertex2f(inner_left, inner_top);
		rlVertex2f(inner_right, inner_top);
		rlVertex2f(inner_right, inner_bottom);

		rlVertex2f(inner_left, inner_top);
		rlVertex2f(inner_right, inner_bottom);
		rlVertex2f(inner_left, inner_bottom);

		// ----- Top strip
		rlVertex2f(inner_left, top);
		rlVertex2f(inner_right, top);
		rlVertex2f(inner_right, inner_top);

		rlVertex2f(inner_left, top);
		rlVertex2f(inner_right, inner_top);
		rlVertex2f(inner_left, inner_top);

		// ----- Bottom strip
		rlVertex2f(inner_left, inner_bottom);
		rlVertex2f(inner_right, inner_bottom);
		rlVertex2f(inner_right, bottom);

		rlVertex2f(inner_left, inner_bottom);
		rlVertex2f(inner_right, bottom);
		rlVertex2f(inner_left, bottom);

		// ----- Left strip
		rlVertex2f(left, inner_top);
		rlVertex2f(inner_left, inner_top);
		rlVertex2f(inner_left, inner_bottom);

		rlVertex2f(left, inner_top);
		rlVertex2f(inner_left, inner_bottom);
		rlVertex2f(left, inner_bottom);

		// ----- Right strip
		rlVertex2f(inner_right, inner_top);
		rlVertex2f(right, inner_top);
		rlVertex2f(right, inner_bottom);

		rlVertex2f(inner_right, inner_top);
		rlVertex2f(right, inner_bottom);
		rlVertex2f(inner_right, inner_bottom);

		// ----- Corner arcs
		auto corner = [&](float cx, float cy, float start_angle)
			{
				float step = slv::math::pi * 0.5f / segments;

				for (int i = 0; i < segments; ++i)
				{
					float a0 = start_angle + step * i;
					float a1 = start_angle + step * (i + 1);

					rlVertex2f(cx, cy);
					rlVertex2f(cx + std::cos(a0) * radius, cy + std::sin(a0) * radius);
					rlVertex2f(cx + std::cos(a1) * radius, cy + std::sin(a1) * radius);
				}
			};

		corner(inner_left, inner_top, slv::math::pi);
		corner(inner_right, inner_top, -slv::math::pi * 0.5f);
		corner(inner_right, inner_bottom, 0.f);
		corner(inner_left, inner_bottom, slv::math::pi * 0.5f);

		rlEnd();
		rlPopMatrix();
	}
	*/

	void draw_rectangle_lines(const slv::mat3& matrix, const slv::vec2<float>& offset, const slv::size<float>& size, const slv::rgb& color, float alpha, float thickness)
	{
		rl_push_mult_matrix(matrix);
		DrawRectangleLinesEx(Rectangle(offset.x, offset.y, size.width, size.height), thickness, rl_color(color, alpha));
		rlPopMatrix();
	}

	/*
	void draw_round_rectangle_lines(const slv::mat3& matrix, const slv::vec2<float>& offset, const slv::size<float>& size, const slv::rgb& color, float alpha, float thickness, float roundness, int segments)
	{
		rl_push_mult_matrix(matrix);
		DrawRectangleRoundedLinesEx(Rectangle(offset.x, offset.y, size.width, size.height), roundness, segments, thickness, rl_color(color, alpha));
		rlPopMatrix();
	}
	*/

	void draw_texture(const slv::texture& texture, const slv::rect<float>& source, const slv::vec2<float>& offset, const slv::mat3& matrix, const slv::rgb& color, float alpha)
	{
		rl_push_mult_matrix(matrix);
		DrawTexturePro(rl_texture(texture), rl_rect(source), Rectangle(offset.x, offset.y, source.width, source.height), Vector2(0.f, 0.f), 0.f, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_line(const slv::mat3& matrix, const slv::vec2<float>& start_pos, const slv::vec2<float>& end_pos, const slv::rgb& color, float alpha, float thickness)
	{
		rl_push_mult_matrix(matrix);
		DrawLineEx(rl_vector(start_pos), rl_vector(end_pos), thickness, rl_color(color, alpha));
		rlPopMatrix();
	}

	void draw_circle(const slv::mat3& matrix, const slv::rgb& color, float alpha, float radius)
	{
		rl_push_mult_matrix(matrix);
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