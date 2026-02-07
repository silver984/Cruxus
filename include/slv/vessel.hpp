#pragma once

#include <slv/core/types.hpp>
#include <slv/core/colors.hpp>
#include <vector>
#include <string>
#include <memory>

namespace slv
{
	class Vessel : public std::enable_shared_from_this<Vessel>
	{
	public:
		virtual ~Vessel() = default;

		inline void add_vessel(const std::shared_ptr<Vessel>& vessel)
		{
			if (!vessel)
			{
				return;
			}

			if (auto old_parent = vessel->m_parent.lock())
			{
				old_parent->remove_vessel(vessel);
			}

			vessel->m_parent = weak_from_this();
			m_vessels.emplace_back(vessel);
		}

		inline void remove_vessel(const std::shared_ptr<Vessel>& vessel)
		{
			std::erase(m_vessels, vessel);
			
			if (vessel)
			{
				vessel->m_parent.reset();
			}
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

		inline std::shared_ptr<Vessel> get_parent() const
		{
			return m_parent.lock();
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
		slv::rgb color = slv::colors::WHITE;

		// Visibility toggle
		// If turned false, this vessel will stop drawing but will keep updating
		bool is_visible = true;

		// Active toggle
		// If turned false, this vessel will stop updating and drawing
		bool is_active = true;

		float time_scale = 1.f;

	protected:
		inline virtual bool init()
		{
			return true;
		};

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
		std::weak_ptr<Vessel> m_parent;
		std::vector<std::shared_ptr<Vessel>> m_vessels;
		std::string m_name;
		bool m_is_init = false;
	};
}