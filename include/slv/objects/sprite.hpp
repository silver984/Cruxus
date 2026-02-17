#pragma once

#include <slv/objects/Vessel.hpp>
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

		inline std::string type() const override
		{
			return "Sprite";
		}

		void set_antialiasing(bool val);

	protected:
		bool init(const slv::game_context& ctx) override;
		void update(float dt, const slv::game_context& ctx) override;
		void draw(const slv::game_context& ctx) const override;

	private:
		std::string m_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		slv::sptr<slv::texture> m_texture = nullptr;
	};
}