#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <string>
#include <memory>

namespace slv
{
	class Sprite : public slv::Vessel
	{
	public:
		static inline Sprite* create(const std::string& file_path)
		{
			Sprite* s = new Sprite(file_path);

			if (!s->base_init())
			{
				s->destroy();
				return nullptr;
			}

			return s;
		}

		inline std::string get_type() const override
		{
			return "Sprite";
		}

		bool is_antialiasing = true;

	protected:
		Sprite(const std::string& file_path)
		{
			m_file_path = file_path;
		}

		bool init() override;
		void update(float dt) override;
		void draw() const override;

	private:
		bool m_antialiasing_check = false;
		std::string m_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		std::shared_ptr<slv::texture> m_texture = nullptr;
	};
}