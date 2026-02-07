#pragma once

#include <slv/vessel.hpp>
#include <memory>
#include <string>

namespace slv
{
	class Game; // forward declare

	class Scene : public slv::Vessel
	{
		friend class slv::Game;

	public:
		template<typename Derived>
		static inline std::shared_ptr<Scene> create()
		{
			static_assert(std::is_base_of_v<Scene, Derived>);

			auto scene = std::make_shared<Derived>();
			if (scene->base_init())
			{
				return scene;
			}
			
			return nullptr;
		}

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