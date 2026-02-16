#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/pointers.hpp>

namespace slv
{
	class Fleet : public slv::Vessel
	{
	public:
		static inline s_ptr<Fleet> create()
		{
			s_ptr<Fleet> fleet = shared<Fleet>();

			if (!fleet->base_init())
			{
				fleet.reset();
				return nullptr;
			}

			return fleet;
		}

		inline std::string get_type() const override
		{
			return "Fleet";
		}

	protected:
		void update(float dt) override {};
		void draw() const override {};
	};
}