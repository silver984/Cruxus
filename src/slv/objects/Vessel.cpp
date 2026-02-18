#include <slv/objects/Vessel.hpp>
#include <slv/game/Window.hpp>
#include <slv/core/math.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

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

	float Vessel::world_rotation() const
	{
		return std::atan2(world_transform_.m_[1][0], world_transform_.m_[0][0]);
	}

	slv::vec2<float> Vessel::world_position() const
	{
		return world_transform_.translation();
	}

	slv::vec2<float> Vessel::world_scale() const
	{
		float sx = std::sqrt(world_transform_.m_[0][0] * world_transform_.m_[0][0] +
							 world_transform_.m_[0][1] * world_transform_.m_[0][1]);

		float sy = std::sqrt(world_transform_.m_[1][0] * world_transform_.m_[1][0] +
							 world_transform_.m_[1][1] * world_transform_.m_[1][1]);

		return slv::vec2<float>(sx, sy);
	}

	slv::size<float> Vessel::world_dimensions() const
	{
		return slv::size<float>(dimensions_.width * world_scale().x, dimensions_.height * world_scale().y);
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

		bool was_resized = ctx.window ? ctx.window->was_resized() : false;
		bool alpha_changed = std::abs(m_last_alpha - alpha) > std::numeric_limits<float>::epsilon();
		if (was_resized || m_last_pos != pos || alpha_changed || m_last_anchor != anchor ||
			m_last_rotation != rotation || m_last_scale != scale || m_last_dimensions != dimensions_)
		{
			m_last_pos = pos;
			m_last_alpha = alpha;
			m_last_anchor = anchor;
			m_last_rotation = rotation;
			m_last_scale = scale;
			m_last_dimensions = dimensions_;
			mark_dirty();
		}

		if (m_is_dirty)
		{
			auto anchor_offset = slv::vec2<float>(anchor.x * dimensions_.width,
												  anchor.y * dimensions_.height);
			float rad = slv::math::deg_to_rad(rotation);

			slv::mat3 T = slv::mat3::translation(pos);
			slv::mat3 R = slv::mat3::rotation(rad);
			slv::mat3 S = slv::mat3::scale(scale);
			slv::mat3 A = slv::mat3::translation(-anchor_offset);

			local_transform_ = T * R * S * A;

			if (auto p = m_parent.lock())
			{
				world_transform_ = p->world_transform_ * local_transform_;
				world_alpha_ = std::clamp(alpha * p->world_alpha_, 0.f, 1.f);
			}
			else
			{
				float ui_scale = ctx.window ? ctx.window->ui_scale() : 1.f;
				slv::mat3 UI = slv::mat3::scale(slv::vec2<float>(ui_scale, ui_scale));
				world_transform_ = UI * local_transform_;
				world_alpha_ = std::clamp(alpha, 0.f, 1.f);
			}

			m_is_dirty = false;
		}

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
		if (!m_is_initialized || !is_visible || world_alpha_ == 0.f)
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

	// private
	void Vessel::mark_dirty()
	{
		if (!m_is_dirty)
		{
			m_is_dirty = true;

			for (auto& child : m_children)
			{
				if (child)
				{
					child->mark_dirty();
				}
			}
		}
	}
}