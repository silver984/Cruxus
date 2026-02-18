#include <slv/objects/Vessel.hpp>
#include <slv/game/Window.hpp>
#include <slv/core/math.hpp>
#include <algorithm>

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

		if (std::find(m_children.begin(), m_children.end(), vessel) != m_children.end())
		{
			// prevent duplicates
			return;
		}

		if (auto old_parent = vessel->parent().lock())
		{
			// remove from old parent
			old_parent->remove(vessel);
		}

		vessel->m_parent = weak_from_this();
		m_children.push_back(vessel);
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

		auto it = std::find(m_children.begin(), m_children.end(), vessel);
		if (it == m_children.end())
		{
			return;
		}

		vessel->m_parent.reset();
		m_children.erase(it);
	}

	void Vessel::destroy()
	{
		if (!m_is_initialized)
		{
			return;
		}

		m_is_initialized = false;

		if (auto parent = m_parent.lock())
		{
			parent->remove(shared_from_this());
		}

		// recursively destroy children
		while (!m_children.empty())
		{
			auto child = m_children.back();
			m_children.pop_back();

			child->m_parent.reset();
			child->destroy();
		}
	}

	// protected
	bool Vessel::base_init(const slv::game_context& ctx)
	{
		if (m_is_initialized)
		{
			return true;
		}

		if (!init(ctx))
		{
			return false;
		}

		m_is_initialized = true;

		return true;
	}

	// protected
	void Vessel::base_update(float dt, const slv::game_context& ctx)
	{
		if (!m_is_initialized || !is_active)
		{
			return;
		}

		// TODO: world transform
		// TODO: negative values

		slv::sptr<Vessel> _parent = parent().lock();

		slv::vec2<float> parent_world_scale = _parent ? _parent->world_scale_ : slv::vec2<float>(1.f, 1.f);
		slv::vec2<float> parent_world_pos = _parent ? _parent->world_pos_ : slv::vec2<float>(0.f, 0.f);
		float parent_world_rotation = _parent ? _parent->world_rotation_ : 0.f;
		float parent_world_alpha = _parent ? _parent->world_alpha_ : 1.f;

		if (!_parent)
		{
			if (auto window = ctx.window)
			{
				world_scale_ = scale * window->ui_scale();
			}
		}
		else
		{
			world_scale_ = scale * parent_world_scale;
		}

		world_pos_ = parent_world_pos + (pos * parent_world_scale);
		world_rotation_ = rotation + parent_world_rotation;
		world_alpha_ = std::clamp(std::clamp(alpha, 0.f, 1.f) * parent_world_alpha, 0.f, 1.f);
		// world_anchor_ = slv::vec2<float>(anchor.x * size_.width, anchor.y * size_.height) * world_scale_;
		world_anchor_ = slv::vec2<float>(anchor.x * scaled_dimensions().width, anchor.y * scaled_dimensions().height);

		float world_dt = dt * time_scale;
		update(world_dt, ctx);

		for (const auto& vessel : m_children)
		{
			if (!vessel)
			{
				continue;
			}
			
			vessel->base_update(world_dt, ctx);
		}
	}

	// protected
	void Vessel::base_draw(const slv::game_context& ctx) const
	{
		if (!m_is_initialized || !is_visible || alpha == 0.f)
		{
			return;
		}

		draw(ctx);

		for (const auto& v : m_children)
		{
			if (!v)
			{
				continue;
			}

			v->base_draw(ctx);
		}
	}

	// private
	bool Vessel::has_ancestor(const slv::sptr<Vessel>& vessel) const
	{
		auto p = parent().lock();

		while (p)
		{
			if (p == vessel)
			{
				return true;
			}

			p = p->parent().lock();
		}

		return false;
	}

	// private
	void Vessel::clean_children()
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), nullptr), m_children.end());
	}
}