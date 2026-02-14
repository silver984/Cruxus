#include <slv/vessel.hpp>
#include <slv/core/math.hpp>
#include <slv/handlers/window_handler.hpp>
#include <algorithm>
#include <cassert>

namespace slv
{
	// protected
	bool Vessel::base_init()
	{
		if (m_is_init)
		{
			return true;
		}

		if (!init())
		{
			return false;
		}

		m_is_init = true;

		return true;
	}

	// protected
	void Vessel::base_update(float dt)
	{
		if (!m_is_init || !is_active)
		{
			return;
		}

		// to do world transform

		slv::vec_2<float> parent_world_scale = m_parent ? m_parent->world_scale_ : slv::vec_2<float>(1.0F, 1.0F);
		slv::vec_2<float> parent_world_pos = m_parent ? m_parent->world_pos_ : slv::vec_2<float>(0.0F, 0.0F);
		float parent_world_rotation = m_parent ? m_parent->world_rotation_ : 0.0F;
		float parent_world_alpha = m_parent ? m_parent->world_alpha_ : 1.0F;

		if (!m_parent)
		{
			world_scale_ = scale * slv::WindowHandler::get().get_ui_scale();
		}
		else
		{
			world_scale_ = scale * parent_world_scale;
		}

		world_pos_ = parent_world_pos + (pos * parent_world_scale);
		world_rotation_ = rotation + parent_world_rotation;
		world_alpha_ = std::clamp(alpha * parent_world_alpha, 0.0F, 1.0F);
		world_anchor_ = slv::vec_2<float>(anchor.x * size_.width,
											anchor.y * size_.height) * world_scale_;

		float world_dt = dt * time_scale;
		update(world_dt);

		// remove all nullptr vessels
		m_vessels.erase(std::remove(m_vessels.begin(), m_vessels.end(), nullptr), m_vessels.end());

		for (const auto& v : m_vessels)
		{
			if (!v)
			{
				continue;
			}
			
			v->base_update(world_dt);
		}
	}

	// protected
	void Vessel::base_draw() const
	{
		if (!m_is_init || !is_active || !is_visible || alpha == 0.0F)
		{
			return;
		}

		draw();

		/*
		* TO INTEGRATE LATER
		float outline_thickness = 1.0F;
		slv::vec_2<float> rect_pos = world_pos_ - world_anchor_;

		std::array<Vector2, 4> corners = {
			Vector2(rect_pos.x - outline_thickness,
					rect_pos.y - outline_thickness),

			Vector2(rect_pos.x + (size_.width * world_scale_.x) + outline_thickness,
					rect_pos.y - outline_thickness),

			Vector2(rect_pos.x + (size_.width * world_scale_.x) + outline_thickness,
					rect_pos.y + (size_.height * world_scale_.y) + outline_thickness),

			Vector2(rect_pos.x - outline_thickness,
					rect_pos.y + (size_.height * world_scale_.y) + outline_thickness)
		};

		for (size_t i = 0; i < corners.size(); i++)
		{
			size_t next = (i + 1) % corners.size();
			DrawLineEx(corners[i], corners[next], outline_thickness, Color(255, 0, 0, 255));
		}
		*/

		for (const auto& v : m_vessels)
		{
			if (!v)
			{
				continue;
			}

			v->base_draw();
		}
	}

	void Vessel::destroy()
	{
		std::string type = get_type();
		std::string name = get_name();

		if (!name.empty())
		{
			type = type + " : \"" + name + "\"";
		}
		
		if (m_parent)
		{
			m_parent->remove_vessel(this);
		}

		delete this;
		
		slv::console_log(slv::LOG_INFO, type.c_str(), "Destroyed");
	}
}