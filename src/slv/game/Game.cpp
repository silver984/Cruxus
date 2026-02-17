#include <slv/game/Game.hpp>

namespace slv
{
	bool Game::init(const std::string& window_title, const slv::size<int>& window_size, int fps, int window_settings)
	{
		if (m_is_init)
		{
			return true;
		}

		if (!m_window.init(window_title, window_size, fps, window_settings))
		{
			return false;
		}

		m_audio_manager.init();
		m_crash_manager.init();

		m_is_init = true;

		return true;
	}

	void Game::run()
	{
		while (m_window.is_open())
		{
			// update

			m_window.update();
			m_input_manager.update();
			float dt = m_window.delta_time();
			m_resource_manager.update(dt);
			auto ctx = context();
			m_scene_manager.update(dt, ctx);
			
			// draw

			m_window.start_draw();
			m_scene_manager.draw(ctx);
			m_window.end_draw();
		}
	}
}