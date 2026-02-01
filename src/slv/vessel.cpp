#include <slv/vessel.hpp>
#include <slv/core/math.hpp>
#include <slv/handlers/window_handler.hpp>
#include <algorithm>

namespace slv
{
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

	void Vessel::base_update(float dt)
	{
		if (!m_is_init || !is_active)
		{
			return;
		}

		// to do world transform

		slv::vec_2<float> parent_world_scale = m_parent ? m_parent->world_scale_ : slv::vec_2<float>(1.f, 1.f);
		slv::vec_2<float> parent_world_pos = m_parent ? m_parent->world_pos_ : slv::vec_2<float>();
		float parent_world_rotation = m_parent ? m_parent->world_rotation_ : 0.f;
		float parent_world_alpha = m_parent ? m_parent->world_alpha_ : 1.f;

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
		world_alpha_ = alpha * parent_world_alpha;

		float world_dt = dt * time_scale;

		update(world_dt);

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

	void Vessel::base_draw() const
	{
		if (!m_is_init || !is_active || !is_visible || alpha == 0.f)
		{
			return;
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