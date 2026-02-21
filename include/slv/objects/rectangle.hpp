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

		inline void set_dimensions(const slv::size<float>& dimensions)
		{
			this->dimensions_ = dimensions;
		}

		inline void set_width(float width)
		{
			this->dimensions_.width = width;
		}

		inline void set_height(float height)
		{
			this->dimensions_.height = height;
		}

		/*
		// TO DO ROUNDNESS
		float roundness = 0.f; // 0 - 1

		enum class outline
		{
			INNER,
			MIDDLE,
			OUTER
		};

		float outline_thickness = 0.f;
		float outline_alpha = 1.f;
		slv::rgb outline_color = slv::color::red;
		outline outline_type = outline::MIDDLE;
		*/

	protected:
		inline bool init(const slv::game_context& ctx) override
		{
			update(0.f, ctx);
			return true;
		}

		// void update(float dt, const slv::game_context& ctx) override;
		void draw(const slv::game_context& ctx) const override;

		/*
	private:
		int m_roundness_segments = 0;
		float m_world_outline_alpha = 0.f;
		float m_outline_offset_factor = 0.f;
		slv::vec2<float> m_outline_offsets{};
		*/
	};
}