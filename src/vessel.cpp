#include <slv/vessel.hpp>
#include <slv/math.hpp>
#include <slv/backend/handlers/window.hpp>
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

		Vessel* parent = get_parent();
		slv::vec_2 parent_local_scale = parent ? parent->local_scale_ : slv::vec_2{ 1.f, 1.f };
		local_scale_ = scale * parent_local_scale * slv::WindowHandler::get().get_ui_scale();

		float local_dt = dt * time_scale;

		update(local_dt);

		for (const auto& v : m_vessels)
		{
			if (!v)
			{
				continue;
			}
			
			v->base_update(local_dt);
		}

		// clean up null children
		m_vessels.erase(std::remove_if(m_vessels.begin(), m_vessels.end(),
						[](const auto& v)
						{
							return !v;
						}),
						m_vessels.end());
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