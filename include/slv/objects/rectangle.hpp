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
		static inline Rectangle* create(const slv::rect<float>& rect, const slv::rgb& color = slv::red)
		{
			Rectangle* r = new Rectangle(rect, color);

			if (!r->base_init())
			{
				r->destroy();
				return nullptr;
			}

			return r;
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
		Rectangle(const slv::rect<float>& rect, const slv::rgb& color)
		{
			m_rect = rect;
			pos = slv::vec_2<float>(rect.x, rect.y);
			size_ = slv::size<float>(rect.width, rect.height);
			this->color = color;
		}

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