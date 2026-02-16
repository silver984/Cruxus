#include <slv/vessel.hpp>
#include <slv/core/math.hpp>
#include <slv/handlers/window_handler.hpp>
#include <algorithm>
#include <cassert>

namespace slv
{
	void Vessel::add(const slv::sptr<Vessel>& vessel)
	{
		if (!vessel)
		{
			return;
		}

		auto self = shared_from_this();

		if (vessel == self)
		{
			// prevent self-parenting
			return;
		}

		if (vessel->has_ancestor(self))
		{
			// prevent hierarchy cycle
			return;
		}

		if (std::find(m_vessels.begin(), m_vessels.end(), vessel) != m_vessels.end())
		{
			// prevent duplicates
			return;
		}

		if (auto old_parent = vessel->get_parent().lock())
		{
			// remove from old parent
			old_parent->remove(vessel);
		}

		vessel->m_parent = weak_from_this();
		m_vessels.push_back(vessel);
	}

	void Vessel::remove(const slv::sptr<Vessel>& vessel)
	{
		if (!vessel)
		{
			return;
		}

		auto self = shared_from_this();

		if (vessel == self)
		{
			// prevent self-remove
			return;
		}

		auto it = std::find(m_vessels.begin(), m_vessels.end(), vessel);
		if (it == m_vessels.end())
		{
			return;
		}

		vessel->m_parent.reset();
		m_vessels.erase(it);
	}

	void Vessel::destroy()
	{
		if (!m_is_init)
		{
			return;
		}

		m_is_init = false;

		if (auto parent = m_parent.lock())
		{
			parent->remove(shared_from_this());
		}

		on_destroyed();

		// recursively destroy children
		while (!m_vessels.empty())
		{
			auto child = m_vessels.back();
			m_vessels.pop_back();

			child->m_parent.reset();
			child->destroy();
		}
	}

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

		slv::sptr<Vessel> parent = get_parent().lock();

		slv::vec2<float> parent_world_scale = parent ? parent->world_scale_ : slv::vec2<float>(1.0F, 1.0F);
		slv::vec2<float> parent_world_pos = parent ? parent->world_pos_ : slv::vec2<float>(0.0F, 0.0F);
		float parent_world_rotation = parent ? parent->world_rotation_ : 0.0F;
		float parent_world_alpha = parent ? parent->world_alpha_ : 1.0F;

		if (!parent)
		{
			world_scale_ = scale * SLV_WINDOW_HND.get_ui_scale();
		}
		else
		{
			world_scale_ = scale * parent_world_scale;
		}

		world_pos_ = parent_world_pos + (pos * parent_world_scale);
		world_rotation_ = rotation + parent_world_rotation;
		world_alpha_ = std::clamp(alpha * parent_world_alpha, 0.0F, 1.0F);
		world_anchor_ = slv::vec2<float>(anchor.x * size_.width,
											anchor.y * size_.height) * world_scale_;

		float world_dt = dt * time_scale;
		update(world_dt);

		// remove all nullptr vessels
		m_vessels.erase(std::remove(m_vessels.begin(), m_vessels.end(), nullptr), m_vessels.end());

		for (const auto& vessel : m_vessels)
		{
			if (!vessel)
			{
				continue;
			}
			
			vessel->base_update(world_dt);
		}
	}

	// protected
	void Vessel::base_draw() const
	{
		if (!m_is_init || !is_visible || alpha == 0.0F)
		{
			return;
		}

		draw();

		/*
		* TO INTEGRATE LATER
		float outline_thickness = 1.0F;
		slv::vec2<float> rect_pos = world_pos_ - world_anchor_;

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

	// private
	bool Vessel::has_ancestor(const slv::sptr<Vessel>& vessel) const
	{
		auto p = get_parent().lock();

		while (p)
		{
			if (p == vessel)
			{
				return true;
			}

			p = p->get_parent().lock();
		}

		return false;
	}
}