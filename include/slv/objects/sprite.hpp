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

		inline std::string get_type() const override
		{
			return "Sprite";
		}

		void set_antialiasing(bool val);

	protected:
		bool init() override;
		void update(float dt) override;
		void draw() const override;

	private:
		bool m_antialiasing_check = false;
		std::string m_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		slv::sptr<slv::texture> m_texture = nullptr;
	};
}