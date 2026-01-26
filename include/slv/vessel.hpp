#pragma once

#include <slv/types.hpp>
#include <slv/colors.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

namespace slv
{
	class Vessel
	{
	public:
		virtual ~Vessel() = default;
		virtual void update(float dt) = 0;

		inline void add_vessel(std::shared_ptr<Vessel> vessel)
		{
			if (vessel)
			{
				vessel->m_parent = this;
				m_vessels.push_back(vessel);
			}
		}

		inline const slv::size get_size() const
		{
			return size_;
		}

		inline const slv::size get_scaled_size() const
		{
			return slv::size{ size_.width * world_scale_.x, size_.height * world_scale_.y };
		}

		inline Vessel* get_parent() const
		{
			return m_parent;
		}

		slv::vec_2 pos{}; // Position on the screen
		
		slv::vec_2 anchor{ 0.5f, 0.5f }; // Anchor point [0, 1]
		
		// Scale
		// (1, 1) on default
		slv::vec_2 scale{ 1.f, 1.f };
		
		float rotation = 0.f; // Rotation in degrees
		
		float alpha = 1.f; // Opacity [0, 1]

		// Color of this vessel [0, 255]
		// Not including the alpha channel
		// White by default
		slv::rgb_8 color = slv::WHITE;

		// Visibility toggle
		// If turned false, this vessel will stop drawing but will keep updating
		bool is_visible = true;

		// Active toggle
		// If turned false, this vessel will stop updating and drawing
		bool is_active = true;

		float time_scale = 1.f;

	protected:
		virtual bool init()
		{
			return true;
		};

		virtual void draw() const = 0;
		bool base_init();
		void base_update(float dt);
		void base_draw() const;

		slv::size size_; // Width and height of this vessel
		slv::vec_2 world_scale_{ 1.f, 1.f };
		slv::vec_2 world_pos_{};
		float world_rotation_ = 0.f;
		float world_alpha_ = 1.f;

	private:
		Vessel* m_parent = nullptr;
		std::vector<std::shared_ptr<Vessel>> m_vessels;
		bool m_is_init = false;
	};
}