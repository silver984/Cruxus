#pragma once

#include <slv/vessel.hpp>
#include <slv/types.hpp>
#include <string>
#include <string_view>
#include <memory>

namespace slv
{
	class Sprite : public slv::Vessel
	{
	public:
		Sprite(std::string_view file_path)
		{
			m_file_path = file_path;
		}

		static inline std::shared_ptr<Sprite> create(std::string_view file_path)
		{
			std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(file_path);

			if (spr->base_init())
			{
				return spr;
			}

			return nullptr;
		}

		void update(float dt) override;

		bool is_antialiasing = true;

	protected:
		bool init() override;
		void draw() const override;

	private:
		bool m_antialiasing_check = false;
		std::string m_file_path;
		slv::rect m_source{};
		slv::rect m_dest{};
		std::shared_ptr<slv::texture> m_texture = nullptr;
	};
}