#pragma once

#include <slv/vessel.hpp>

namespace slv
{
	class Fleet : public slv::Vessel
	{
	public:
		static inline std::shared_ptr<Fleet> create()
		{
			std::shared_ptr<Fleet> fleet = std::make_shared<Fleet>();

			if (fleet->base_init())
			{
				return fleet;
			}

			return nullptr;
		}

		void update(float dt) override {};

		std::string get_type() const override
		{
			return "Fleet";
		}

	protected:
		void draw() const override {};
	};
}