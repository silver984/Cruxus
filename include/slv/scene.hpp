#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/pointers.hpp>
#include <string>

namespace slv
{
	class Game; // forward declare

	class Scene : public slv::Vessel
	{
		friend class slv::Game;

	public:
		inline std::string get_type() const override
		{
			return "Scene";
		}

	protected:
		inline slv::Game* get_game()
		{
			return m_game;
		}

	private:
		slv::Game* m_game = nullptr;
	};
}