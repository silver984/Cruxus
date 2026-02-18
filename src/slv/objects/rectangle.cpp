#include <slv/objects/rectangle.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/math.hpp>
#include <cstdint>
#include <algorithm>

namespace slv
{
	// protected
	void Rectangle::update(float dt, const slv::game_context& ctx)
	{
		float width = this->size_.width * this->world_scale_.x;
		float height = this->size_.height * this->world_scale_.y;
		m_rect = slv::rect<float>(this->world_pos_.x, this->world_pos_.y, width, height);
		m_world_line_size = std::max(0.0F, line_size);
		m_world_line_alpha = std::clamp(line_alpha, 0.0F, 1.0F) * this->world_alpha_;

		slv::vec2<float> lined_rect_pos = this->world_pos_ - this->world_anchor_;
		auto rect_dim = this->scaled_dimensions();
		std::array<slv::vec2<float>, 4> corners = {
			slv::vec2<float>(lined_rect_pos.x, lined_rect_pos.y),
			slv::vec2<float>(lined_rect_pos.x + rect_dim.width, lined_rect_pos.y),
			slv::vec2<float>(lined_rect_pos.x + rect_dim.width, lined_rect_pos.y + rect_dim.height),
			slv::vec2<float>(lined_rect_pos.x, lined_rect_pos.y + rect_dim.height)
		};

		for (size_t i = 0; i < m_corners.size(); i++)
		{
			m_corners[i] = slv::math::rotate(corners[i], this->world_pos_, this->world_rotation_);
		}
	}

	// protected
	void Rectangle::draw(const slv::game_context& ctx) const
	{
		slv::raylib::draw_rectangle(m_rect, this->world_anchor_, this->color, this->world_alpha_, this->world_rotation_);
		
		// draw lines
		if (m_world_line_size > 0.0F && m_world_line_alpha > 0.0F)
		{
			size_t corners_count = m_corners.size();
			for (size_t i = 0; i < corners_count; i++)
			{
				size_t next = (i + 1) % corners_count;
				slv::raylib::draw_line(m_corners[i], m_corners[next], m_world_line_size, m_world_line_alpha, line_color);
				slv::raylib::draw_circle(m_corners[i], m_world_line_size / 2.0F, m_world_line_alpha, line_color);
			}
		}
	}
}