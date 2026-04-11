#include <slv/objects/rectangle.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/math.hpp>
#include <algorithm>
#include <cmath>

namespace slv
{
		/*
	void Rectangle::update(float dt, const slv::game_context& ctx)
	{
		roundness = std::clamp(roundness, 0.f, 1.f);
		m_roundness_segments = static_cast<int>(std::round(std::min(this->dimensions_.width, this->dimensions_.height)) / 10.f);

		outline_thickness = std::max(0.f, outline_thickness);
		outline_alpha = std::clamp(outline_alpha, 0.f, 1.f);
		m_world_outline_alpha = outline_alpha * this->world_alpha();

		switch (outline_type)
		{
		case outline::MIDDLE:
			m_outline_offset_factor = outline_thickness;
			break;

		case outline::OUTER:
			if (roundness == 0.f)
			{
				m_outline_offset_factor = outline_thickness * 2.f;
			}
			else
			{
				m_outline_offset_factor = 0.f;
			}
			break;

		default: // INNER
			if (roundness == 0.f)
			{
				m_outline_offset_factor = 0.f;
			}
			else
			{
				m_outline_offset_factor = outline_thickness * 2.f;
			}
			break;
		}

		m_outline_offsets = -slv::vec2<float>(m_outline_offset_factor, m_outline_offset_factor) / 2.f;
	}
		*/

	// protected
	void Rectangle::draw(const slv::game_context& ctx) const
	{
		auto world_transform = this->world_transform();
		auto world_alpha = this->world_alpha();

		slv::raylib::draw_rectangle(world_transform, this->dimensions_, this->color, world_alpha);
		
		/*
		if (roundness == 0.f)
		{
		}
		else // with roundness
		{
			slv::raylib::draw_round_rectangle(world_transform, this->dimensions_, this->color, world_alpha, roundness, 4);
		}

		// draw outline
		if (outline_thickness > 0.f && m_world_outline_alpha > 0.f)
		{
			if (roundness == 0.f)
			{
				slv::raylib::draw_rectangle_lines(world_transform, m_outline_offsets, this->dimensions_ + m_outline_offset_factor, this->outline_color, m_world_outline_alpha, outline_thickness);
			}
			else // with roundness
			{
				slv::raylib::draw_round_rectangle_lines(world_transform, -m_outline_offsets, this->dimensions_ - m_outline_offset_factor, this->outline_color, m_world_outline_alpha, outline_thickness, roundness, m_roundness_segments);
			}
		}
		*/
	}
}