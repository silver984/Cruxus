#include <slv/core/wrappers/raylib.hpp>
#include <raylib.h>

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

	RenderTexture2D rl_render_texture(const slv::render_texture& render_texture)
	{
		return { render_texture.id, rl_texture(render_texture.tex), rl_texture(render_texture.depth) };
	}

	Vector2 rl_vector(const slv::vec_2<float>& vec_2)
	{
		return { vec_2.x, vec_2.y };
	}

	// ----------------------------------------------------- //

	slv::texture slv_texture(const Texture& texture)
	{
		return { texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
	}
}

namespace slv::raylib
{
	void draw_rectangle(const slv::rect<float>& rect, const slv::vec_2<float>& anchor, const slv::rgb& color, float alpha, float rotation)
	{
		DrawRectanglePro(rl_rect(rect), rl_vector(anchor), rotation, rl_color(color, alpha));
	}

	void draw_texture(const slv::texture& texture, const slv::rect<float>& source, const slv::rect<float>& dest, const slv::vec_2<float>& anchor, float rotation, float alpha, const slv::rgb& color)
	{
		DrawTexturePro(rl_texture(texture), rl_rect(source), rl_rect(dest), rl_vector(anchor), rotation, rl_color(color, alpha));
	}

	void draw_line(const slv::vec_2<float>& start_pos, const slv::vec_2<float>& end_pos, const slv::rgb& color, float alpha, float thickness)
	{
		DrawLineEx(rl_vector(start_pos), rl_vector(end_pos), thickness, rl_color(color, alpha));
	}

	void set_texture_antialiasing(const slv::texture& texture, bool toggle)
	{
		SetTextureFilter(rl_texture(texture), toggle ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}

	slv::render_texture load_render_texture(unsigned int width, unsigned int height)
	{
		RenderTexture2D render_texture_rl = LoadRenderTexture(width, height);
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
}