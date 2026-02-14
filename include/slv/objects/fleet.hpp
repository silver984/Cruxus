#pragma once

#include <slv/vessel.hpp>

namespace slv
{
	class Fleet : public slv::Vessel
	{
	public:
		static inline Fleet* create()
		{
			Fleet* f = new Fleet();

			if (!f->base_init())
			{
				f->destroy();
				return nullptr;
			}

			return f;
		}

		void update(float dt) override {};

		inline std::string get_type() const override
		{
			return "Fleet";
		}

	protected:
		void draw() const override {};
	};
}