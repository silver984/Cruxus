#pragma once

#include <slv/types.hpp>
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
		virtual bool init() = 0;
		virtual void update(float dt) = 0;

		// Subclasses of Vessel must use the world transform instead of the local transform
		virtual void draw() const = 0;

		void add_child(std::shared_ptr<Vessel> vessel)
		{
			m_vessels.push_back(vessel);
		}

		slv::transform transform;
		slv::size dimensions; // Width and height of this vessel

		// Color of this vessel [0, 1]
		// Not including the alpha channel
		slv::rgb_f color;

		bool is_visible = true; // Visibility toggle for this vessel

		// Active toggle for this vessel
		// If turned false, this vessel will stop updating and drawing
		bool is_active = true;

	protected:
		const slv::transform& get_world_transform() const
		{
			return m_world_transform;
		}

		bool base_init();
		void base_update(float dt, const slv::transform& parent_transform);
		void base_draw();

	private:
#ifdef SLV_DEBUG
		static constexpr const char* m_class_name = "Vessel";
#endif
		slv::transform m_world_transform;
		std::vector<std::shared_ptr<Vessel>> m_vessels;
		bool m_is_init = false;
	};
}