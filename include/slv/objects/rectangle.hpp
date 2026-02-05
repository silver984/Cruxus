#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types.hpp>
#include <slv/core/colors.hpp>
#include <memory>
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

		static inline std::shared_ptr<Rectangle> create(const slv::rect<float>& rect, const slv::rgb& color = slv::colors::RED)
		{
			std::shared_ptr<Rectangle> rectangle = std::make_shared<Rectangle>(rect, color);
			
			if (rectangle->base_init())
			{
				return rectangle;
			}

			return nullptr;
		}

		std::string get_type() const override
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

	protected:
		void update(float dt) override;
		void draw() const override;

	private:
		slv::rect<float> m_rect{};
	};
}