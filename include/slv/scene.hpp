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
		template<typename Derived>
		static inline s_ptr<Derived> create()
		{
			static_assert(std::is_base_of_v<Scene, Derived>);

			s_ptr<Derived> scene = shared<Derived>();

			if (!scene->base_init())
			{
				scene.reset();
				return nullptr;
			}
			
			return scene;
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