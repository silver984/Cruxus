#pragma once

#include <slv/core/types/primitives.hpp>
#include <slv/core/types/colors.hpp>
#include <slv/core/console_log.hpp>
#include <vector>
#include <string>

namespace slv
{
	class Vessel
	{
	public:
		inline void add_vessel(Vessel* vessel)
		{
			if (!vessel || vessel == this)
			{
				return;
			}

			if (Vessel* old_parent = vessel->m_parent)
			{
				// remove from old parent
				old_parent->remove_vessel(vessel);
			}

			vessel->m_parent = this;
			m_vessels.push_back(vessel);
		}

		inline void remove_vessel(Vessel* vessel)
		{
			if (!vessel)
			{
				return;
			}

			auto it = std::find(m_vessels.begin(), m_vessels.end(), vessel);
			if (it == m_vessels.end())
			{
				return;
			}

			vessel->m_parent = nullptr;
			m_vessels.erase(it);
		}

		inline size_t get_vessel_count() const
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

		inline Vessel* get_parent() const
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

		void destroy();

		slv::vec_2<float> pos{}; // Position on the screen
		
		slv::vec_2<float> anchor{ 0.5F, 0.5F }; // Anchor point [0, 1]
		
		// Scale
		// (1, 1) on default
		slv::vec_2<float> scale{ 1.0F, 1.0F };
		
		float rotation = 0.0F; // Rotation in degrees
		
		float alpha = 1.0F; // Opacity [0, 1]

		// Color of this vessel [0, 255]
		// Not including the alpha channel
		// White by default
		slv::rgb color = slv::white;

		// Visibility toggle
		// If turned false, this vessel will stop drawing but will keep updating
		bool is_visible = true;

		// Active toggle
		// If turned false, this vessel will stop updating and drawing
		bool is_active = true;

		float time_scale = 1.f;

	protected:
		Vessel() = default;
		virtual ~Vessel()
		{
			for (Vessel* v : m_vessels)
			{
				v->destroy();
				v = nullptr;
			}

			m_vessels.clear();
		}

		inline virtual bool init()
		{
			return true;
		}

		virtual void update(float dt) = 0;
		virtual void draw() const = 0;
		bool base_init();
		void base_update(float dt);
		void base_draw() const;

		slv::size<float> size_; // Width and height of this vessel
		slv::vec_2<float> world_scale_{ 1.0F, 1.0F };
		slv::vec_2<float> world_pos_{};
		slv::vec_2<float> world_anchor_{ 0.5F, 0.5F };
		float world_rotation_ = 0.0F;
		float world_alpha_ = 1.0F;

	private:
		Vessel* m_parent = nullptr;
		std::vector<Vessel*> m_vessels;
		std::string m_name;
		bool m_is_init = false;
	};
}