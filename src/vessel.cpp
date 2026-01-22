#include <slv/vessel.hpp>
#include <slv/math.hpp>
#ifdef SLV_DEBUG
#include <slv/debug/log.hpp>
#endif
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

		float mod_dt = dt * time_scale;

		update(mod_dt);

		for (const auto& v : m_vessels)
		{
			if (!v)
			{
				continue;
			}
			
			v->base_update(mod_dt);
		}

		// clean up null children
		m_vessels.erase(std::remove_if(m_vessels.begin(), m_vessels.end(),
						[](const auto& v)
						{
							return !v;
						}),
						m_vessels.end());
	}

	void Vessel::base_draw()
	{
		if (!m_is_init || !is_active || !is_visible)
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

		// clean up null children
		m_vessels.erase(std::remove_if(m_vessels.begin(), m_vessels.end(),
						[](const auto& v)
						{
							return !v;
						}),
						m_vessels.end());
	}
}