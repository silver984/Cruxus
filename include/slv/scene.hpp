#pragma once

#include <slv/vessel.hpp>
#include <memory>
#include <string>

namespace slv
{
	class Game; // forward declare

	class Scene : public slv::Vessel
	{
	public:
		template<typename Derived>
		static inline std::unique_ptr<Scene> create()
		{
			static_assert(std::is_base_of_v<Scene, Derived>, "Scene must derive from slv::Scene");
			
			auto scene = std::make_unique<Derived>();
			if (scene->base_init())
			{
				return scene;
			}

			return nullptr;
		}

		std::string get_type() const override
		{
			return "Scene";
		}

		friend class slv::Game;

	protected:
		slv::Game* get_game()
		{
			return m_game;
		}

	private:
		slv::Game* m_game = nullptr;
	};
}