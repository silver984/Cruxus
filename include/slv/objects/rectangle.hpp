#pragma once

#include <slv/objects/Vessel.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/colors.hpp>
#include <string>
#include <array>

namespace slv
{
	class Rectangle : public slv::Vessel
	{
	public:
		Rectangle(const slv::size<float>& size, const slv::rgb& color)
		{
			this->dimensions_ = slv::size<float>(size.width, size.height);
			this->color = color;
		}

		inline std::string type() const override
		{
			return "Rectangle";
		}

		inline void set_size(const slv::size<float>& size)
		{
			this->dimensions_ = size;
		}

		inline void set_width(float width)
		{
			this->dimensions_.width = width;
		}

		inline void set_height(float height)
		{
			this->dimensions_.height = height;
		}

		float outline_thickness = 0.f;
		float outline_alpha = 1.f;
		slv::rgb outline_color = slv::color::red;

	protected:
		inline bool init(const slv::game_context& ctx) override
		{
			update(0.f, ctx);
			return true;
		}

		void draw(const slv::game_context& ctx) const override;
	};
}