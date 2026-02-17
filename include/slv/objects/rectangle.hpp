#pragma once

#include <slv/objects/Vessel.hpp>
#include <slv/core/types/primitives.hpp>
#include <string>

namespace slv
{
	class Rectangle : public slv::Vessel
	{
	public:
		Rectangle(const slv::rect<float>& rect, const slv::rgb& color)
		{
			m_rect = rect;
			pos = slv::vec2<float>(rect.x, rect.y);
			size_ = slv::size<float>(rect.width, rect.height);
			this->color = color;
		}

		inline std::string type() const override
		{
			return "Rectangle";
		}

		inline void set_size(const slv::size<float>& size)
		{
			size_ = size;
		}

		inline void set_width(float width)
		{
			size_.width = width;
		}

		inline void set_height(float height)
		{
			size_.height = height;
		}

		float outline_size = 0.0F;
		float outline_alpha = 0.0F;

	protected:
		inline bool init(const slv::game_context& ctx) override
		{
			update(0.0F, ctx);
			return true;
		}

		void update(float dt, const slv::game_context& ctx) override;
		void draw(const slv::game_context& ctx) const override;

	private:
		slv::rect<float> m_rect{};
	};
}