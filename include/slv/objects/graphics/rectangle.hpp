#pragma once

#include <slv/vessel.hpp>
#include <slv/types.hpp>
#include <slv/colors.hpp>
#include <memory>

namespace slv
{
	class Rectangle : public slv::Vessel
	{
	public:
		Rectangle(const slv::rect& rect, const slv::rgb_8& color)
		{
			m_rect = rect;
			pos = slv::vec_2{ rect.x, rect.y };
			size_ = slv::size{ rect.width, rect.height };
			this->color = color;
		}

		static inline std::shared_ptr<Rectangle> create(const slv::rect& rect, const slv::rgb_8& color = slv::RED)
		{
			std::shared_ptr<Rectangle> rectangle = std::make_shared<Rectangle>(rect, color);
			
			if (rectangle->base_init())
			{
				return rectangle;
			}

			return nullptr;
		}

		void update(float dt) override;

		inline void set_size(const slv::size& size)
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
		void draw() const override;

	private:
		slv::rect m_rect{};
	};
}