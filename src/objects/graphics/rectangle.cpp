#include <slv/objects/graphics/rectangle.hpp>
#include <slv/objects/graphics/backend/draw.hpp>
#include <slv/debug/log.hpp>

namespace slv
{
	void Rectangle::update(float dt)
	{
		float width = size_.width * local_scale_.x;
		float height = size_.height * local_scale_.y;
		m_rect = slv::rect{ pos.x, pos.y, width, height };
	}

	void Rectangle::draw() const
	{
		slv::draw_rectangle(m_rect, anchor, color, alpha, rotation);
	}
}