#include <slv/vessel.hpp>
#include <slv/core/math.hpp>
#include <slv/handlers/window_handler.hpp>
#include <algorithm>
#include <raylib.h>
#include <array>

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

		float world_dt = dt * time_scale;
		update(world_dt);

		// to do world transform

		auto parent = m_parent.lock();

		slv::vec_2<float> parent_world_scale = parent ? parent->world_scale_ : slv::vec_2<float>(1.0F, 1.0F);
		slv::vec_2<float> parent_world_pos = parent ? parent->world_pos_ : slv::vec_2<float>();
		float parent_world_rotation = parent ? parent->world_rotation_ : 0.0F;
		float parent_world_alpha = parent ? parent->world_alpha_ : 1.0F;

		if (!parent)
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

		// erase null children
		m_vessels.erase(std::remove_if(m_vessels.begin(), m_vessels.end(),
						[](const auto& v) { return !v; }),
						m_vessels.end());

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

		// to integrate for Rectangle

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

		draw();

		for (const auto& v : m_vessels)
		{
			if (!v)
			{
				continue;
			}

			v->base_draw();
		}
	}
}