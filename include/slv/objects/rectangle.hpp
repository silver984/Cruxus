#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/colors.hpp>
#include <string>

namespace slv
{
	class Rectangle : public slv::Vessel
	{
	public:
		Rectangle(const slv::rect<float>& rect, const slv::rgb& color)
		{
			m_rect = rect;
			pos = slv::vec_2<float>(rect.x, rect.y);
			size_ = slv::size<float>(rect.width, rect.height);
			this->color = color;
		}

		static inline s_ptr<Rectangle> create(const slv::rect<float>& rect, const slv::rgb& color = slv::red)
		{
			s_ptr<Rectangle> rectangle = shared<Rectangle>(rect, color);

			if (!rectangle->base_init())
			{
				rectangle.reset();
				return nullptr;
			}

			return rectangle;
		}

		inline std::string get_type() const override
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
		inline bool init() override
		{
			update(0.0F);
			return true;
		}

		void update(float dt) override;
		void draw() const override;

	private:
		slv::rect<float> m_rect{};
	};
}