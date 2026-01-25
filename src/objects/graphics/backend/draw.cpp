#include <slv/objects/graphics/backend/draw.hpp>
#include <raylib.h>

namespace slv
{
	void draw_rectangle(const slv::rect& rect, const slv::vec_2& anchor, const slv::rgb_8& color, float alpha, float rotation)
	{
		Rectangle _rect{ rect.x, rect.y, rect.width, rect.height };
		Vector2 origin{ rect.width * anchor.x, rect.height * anchor.y };
		uint8_t a = static_cast<uint8_t>(255.f * alpha);
		Color _color{ color.r, color.g, color.g, a };
		DrawRectanglePro(_rect, origin, rotation, _color);
	}
}