#include <slv/objects/rectangle.hpp>
#include <slv/core/wrappers/raylib.hpp>

namespace slv
{
	void Rectangle::update(float dt)
	{
		float width = size_.width * world_scale_.x;
		float height = size_.height * world_scale_.y;
		m_rect = slv::rect<float>(world_pos_.x, world_pos_.y, width, height);
	}

	void Rectangle::draw() const
	{
		slv::raylib::draw_rectangle(m_rect, anchor, color, world_alpha_, world_rotation_);
	}
}