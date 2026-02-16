#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <string>

namespace slv
{
	class Sprite : public slv::Vessel
	{
	public:
		Sprite(const std::string& file_path)
		{
			m_file_path = file_path;
		}

		static inline s_ptr<Sprite> create(const std::string& file_path)
		{
			s_ptr<Sprite> sprite = shared<Sprite>(file_path);

			if (!sprite->base_init())
			{
				sprite.reset();
				return nullptr;
			}

			return sprite;
		}

		inline std::string get_type() const override
		{
			return "Sprite";
		}

		bool is_antialiasing = true;

	protected:
		bool init() override;
		void update(float dt) override;
		void draw() const override;

	private:
		bool m_antialiasing_check = false;
		std::string m_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		s_ptr<slv::texture> m_texture = nullptr;
	};
}