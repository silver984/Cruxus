#include <slv/objects/graphics/backend/rl_wrappers.hpp>
#include <raylib.h>

namespace
{
	Rectangle rl_rect(const slv::rect& rect)
	{
		return Rectangle{ rect.x, rect.y, rect.width, rect.height };
	}

	Color rl_color(const slv::rgb_8& color, float alpha)
	{
		return Color{ color.r, color.g, color.g, static_cast<uint8_t>(255.f * alpha) };
	}

	Texture rl_texture(const slv::texture& texture)
	{
		return Texture{ texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
	}
}

namespace slv::raylib
{
	void draw_rectangle(const slv::rect& rect, const slv::vec_2& anchor, const slv::rgb_8& color, float alpha, float rotation)
	{
		Vector2 origin{ rect.width * anchor.x, rect.height * anchor.y };
		DrawRectanglePro(rl_rect(rect), origin, rotation, rl_color(color, alpha));
	}

	void draw_texture(const slv::texture& texture, const slv::rect& source, const slv::rect& dest, const slv::vec_2& anchor, float rotation, float alpha, const slv::rgb_8& color)
	{
		Vector2 origin{ dest.width * anchor.x, dest.height * anchor.y };
		DrawTexturePro(rl_texture(texture), rl_rect(source), rl_rect(dest), origin, rotation, rl_color(color, alpha));
	}

	void set_texture_antialiasing(const slv::texture& texture, bool toggle)
	{
		SetTextureFilter(rl_texture(texture), toggle ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}