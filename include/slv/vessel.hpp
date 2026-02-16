#pragma once

#include <slv/core/types/primitives.hpp>
#include <slv/core/types/colors.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/console_log.hpp>
#include <vector>
#include <string>
#include <memory>

namespace slv
{
	class Vessel : public std::enable_shared_from_this<Vessel>
	{
	public:
		Vessel() = default;
		virtual ~Vessel() = default;

		inline void add(const s_ptr<Vessel>& vessel)
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

		inline void remove(const s_ptr<Vessel>& vessel)
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

		inline void destroy()
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
				auto& child = m_vessels.back();
				m_vessels.pop_back();

				child->m_parent.reset();
				child->destroy();
			}
		}

		inline size_t get_count() const
		{
			return m_vessels.size();
		}

		inline slv::size<float> get_size() const
		{
			return size_;
		}

		inline slv::size<float> get_scaled_size() const
		{
			return { size_.width * world_scale_.x, size_.height * world_scale_.y };
		}

		inline w_ptr<Vessel> get_parent() const
		{
			return m_parent;
		}

		inline void set_name(const std::string& name)
		{
			m_name = name;
		}

		inline std::string get_name() const
		{
			return m_name;
		}

		inline virtual std::string get_type() const
		{
			return "Vessel";
		}

		slv::vec_2<float> pos{};
		slv::vec_2<float> anchor{ 0.5F, 0.5F };
		slv::vec_2<float> scale{ 1.0F, 1.0F };
		float rotation = 0.0F;
		float alpha = 1.0F;
		slv::rgb color = slv::white;
		bool is_visible = true;
		bool is_active = true;
		float time_scale = 1.0F;

	protected:
		inline virtual bool init()
		{
			return true;
		}

		virtual void update(float dt) = 0;
		virtual void draw() const = 0;
		virtual void on_destroyed() {}
		bool base_init();
		void base_update(float dt);
		void base_draw() const;

		slv::size<float> size_;
		slv::vec_2<float> world_scale_{ 1.0F, 1.0F };
		slv::vec_2<float> world_pos_{};
		slv::vec_2<float> world_anchor_{ 0.5F, 0.5F };
		float world_rotation_ = 0.0F;
		float world_alpha_ = 1.0F;

	private:
		bool has_ancestor(const s_ptr<Vessel>& vessel) const
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

		w_ptr<Vessel> m_parent;
		std::vector<s_ptr<Vessel>> m_vessels;
		std::string m_name;
		bool m_is_init = false;
	};
}