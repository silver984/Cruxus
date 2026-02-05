#include <slv/objects/rectangle.hpp>
#include <slv/core/wrappers/raylib.hpp>

namespace slv
{
	// protected
	void Rectangle::update(float dt)
	{
		float width = this->size_.width * this->world_scale_.x;
		float height = this->size_.height * this->world_scale_.y;
		m_rect = slv::rect<float>(this->world_pos_.x, this->world_pos_.y, width, height);
	}

	// protected
	void Rectangle::draw() const
	{
		slv::raylib::draw_rectangle(m_rect, this->anchor, this->color, this->world_alpha_, this->world_rotation_);
	}
}