#include <slv/game/Game.hpp>
#include <slv/core/console/log.hpp>

namespace slv
{
	bool Game::init(const std::string& window_title, const slv::size<int>& window_size, int fps, int window_settings)
	{
		if (m_is_initialized)
		{
			return true;
		}

		if (!m_window.init(window_title, window_size, fps, window_settings, context()))
		{
			return false;
		}

		m_audio_manager.init();
		m_crash_manager.init();

		m_is_initialized = true;

		return true;
	}

	void Game::run()
	{
		auto ctx = context();

		while (m_window.is_open())
		{
			// update

			float dt = m_window.delta_time();
			m_window.update(dt, ctx);
			m_input_manager.update(dt);
			m_resource_manager.update(dt);
			m_scene_manager.update(dt, ctx);
			
			// draw

			m_window.start_draw();
			m_scene_manager.draw(ctx);
			m_window.end_draw(ctx);
		}

		// cleanup, close window

		m_scene_manager.safely_destroy_scene();
		m_input_manager.clean_cache();
		m_resource_manager.clean_cache();
		m_audio_manager.uninit();
		m_window.uninit();
	}
}