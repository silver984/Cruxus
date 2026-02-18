#pragma once

#include <slv/game/Window.hpp>
#include <slv/game/managers/CrashManager.hpp>
#include <slv/game/managers/AudioManager.hpp>
#include <slv/game/managers/InputManager.hpp>
#include <slv/game/managers/SceneManager.hpp>
#include <slv/game/managers/ResourceManager.hpp>
#include <slv/core/types/game_context.hpp>
#include <string>

namespace slv
{
	class Game
	{
	public:
		Game() = default;
		~Game() = default;
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;

		bool init(const std::string& window_title, const slv::size<int>& window_size, int window_fps, int window_settings);
		void run();

		inline slv::game_context context()
		{
			return slv::game_context(&m_window, &m_scene_manager, &m_input_manager, &m_audio_manager, &m_resource_manager);
		}

	private:
		slv::Window m_window;
		slv::CrashManager m_crash_manager;
		slv::AudioManager m_audio_manager;
		slv::InputManager m_input_manager;
		slv::SceneManager m_scene_manager;
		slv::ResourceManager m_resource_manager;
		bool m_is_initialized = false;
	};
}