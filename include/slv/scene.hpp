#pragma once

#include <slv/vessel.hpp>
#include <memory>

namespace slv
{
	class Game; // forward declare

	class Scene : public Vessel
	{
	public:
		virtual ~Scene() = default;

		template<typename T, typename... Args>
		static std::unique_ptr<Scene> create(Args&&... args)
		{
			static_assert(std::is_base_of_v<Scene, T>, "Must inherit from Scene");
			
			auto scene = std::make_unique<T>();
			if (scene->base_init(std::forward<Args>(args)...))
			{
				return scene;
			}

			return nullptr;
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