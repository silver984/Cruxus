#pragma once

#include <slv/objects/Vessel.hpp>
#include <slv/core/types/primitives.hpp>
#include <string>
#include <array>

namespace slv
{
	enum class line_type
	{
		INNER,
		MIDDLE,
		OUTTER
	};

	class Rectangle : public slv::Vessel
	{
	public:
		Rectangle(const slv::size<float>& size, const slv::rgb& color)
		{
			m_rect.width = size.width;
			m_rect.height = size.height;
			this->size_ = slv::size<float>(size.width, size.height);
			this->color = color;
		}

		inline void set_line_type(line_type type)
		{
			m_line_type = type;
		}

		inline std::string type() const override
		{
			return "Rectangle";
		}

		inline void set_size(const slv::size<float>& size)
		{
			this->size_ = size;
		}

		inline void set_width(float width)
		{
			this->size_.width = width;
		}

		inline void set_height(float height)
		{
			this->size_.height = height;
		}

		float line_size = 0.f;
		float line_alpha = 1.f;
		slv::rgb line_color = slv::color::red;

	protected:
		inline bool init(const slv::game_context& ctx) override
		{
			update(0.f, ctx);
			return true;
		}

		void update(float dt, const slv::game_context& ctx) override;
		void draw(const slv::game_context& ctx) const override;

	private:
		slv::rect<float> m_rect{};
		std::array<slv::vec2<float>, 4> m_corners;
		line_type m_line_type = line_type::MIDDLE;
		float m_world_line_size = 0.f;
		float m_world_line_alpha = 1.f;
	};
}