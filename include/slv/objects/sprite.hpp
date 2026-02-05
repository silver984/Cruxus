#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types.hpp>
#include <string>
#include <memory>

namespace slv
{
	class Sprite : public slv::Vessel
	{
	public:
		Sprite(const std::string& file_path)
		{
			m_file_path = file_path;
		}

		static inline std::shared_ptr<Sprite> create(const std::string& file_path)
		{
			std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(file_path);

			if (spr->base_init())
			{
				return spr;
			}

			return nullptr;
		}

		std::string get_type() const override
		{
			return "Sprite";
		}

		bool is_antialiasing = true;

	protected:
		void update(float dt) override;
		bool init() override;
		void draw() const override;

	private:
		bool m_antialiasing_check = false;
		std::string m_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		std::shared_ptr<slv::texture> m_texture = nullptr;
	};
}