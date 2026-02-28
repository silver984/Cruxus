#pragma once

#include <slv/objects/Vessel.hpp>
#include <slv/core/types/game_context.hpp>
#include <slv/core/types/pointers.hpp>

namespace slv
{
	class Game; // forward declare

	class SceneManager
	{
		friend class slv::Game;

	private:
		SceneManager() = default;
		~SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	public:
		void change_scene(slv::sptr<slv::Vessel>&& new_scene);
		void destroy_current_scene();

		slv::wptr<slv::Vessel> current_scene()
		{
			return m_current_scene;
		}

	private:
		void update(float dt, const slv::game_context& ctx);
		void draw(const slv::game_context& ctx);
		void safely_destroy_scene();

		static constexpr inline const char* M_NAME = "SceneManager";
		slv::sptr<slv::Vessel> m_current_scene = nullptr;
		slv::sptr<slv::Vessel> m_pending_scene = nullptr;
		bool m_destroying_current_scene = false;
	};
}