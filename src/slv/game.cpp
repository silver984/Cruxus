#include <slv/game.hpp>
#include <slv/handlers/window_handler.hpp>
#include <slv/handlers/resource_handler.hpp>
#include <slv/handlers/input_handler.hpp>
#include <slv/handlers/crash_handler.hpp>
#include <slv/handlers/audio_handler.hpp>
#include <slv/core/console_log.hpp>

namespace slv
{
	Game::~Game()
	{
		if (m_current_scene)
		{
			m_current_scene->destroy();
		}

		SLV_AUDIO_HND.uninit();
		SLV_WINDOW_HND.uninit();
	}

	bool Game::init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps,
					bool window_has_vsync, bool is_window_resizable, bool window_starts_fullscreen, bool is_window_borderless, bool is_window_transparent)
	{
		if (m_is_init)
		{
			return true;
		}

		if (!SLV_WINDOW_HND.init(window_title, window_size, fps, window_has_vsync, is_window_resizable, window_starts_fullscreen, is_window_borderless, is_window_transparent))
		{
			return false;
		}

		if (!SLV_AUDIO_HND.init())
		{
			slv::console_log(slv::LOG_ERROR, M_CLASS_NAME,
							 "SLV's audio handler failed");
		}

		if (!slv::CrashHandler::get().init())
		{
			slv::console_log(slv::LOG_WARNING, M_CLASS_NAME,
							 "SLV's crash handler failed or may not be available on this platform");
		}

		m_is_init = true;

		return true;
	}

	void Game::run()
	{
		while (SLV_WINDOW_HND.is_window_open())
		{
			SLV_WINDOW_HND.update();
			SLV_INPUT_HND.update();
			float dt = SLV_WINDOW_HND.get_delta_time();
			SLV_RESOURCE_HND.update(dt);

			// -------------------- UPDATE HERE -------------------- //

			if (m_pending_scene)
			{
				if (m_current_scene)
				{
					m_current_scene->destroy();
					m_current_scene = nullptr;
				}

				m_current_scene = m_pending_scene;
				m_current_scene->m_game = this;

				m_pending_scene = nullptr;
			}

			if (m_current_scene)
			{
				m_current_scene->base_update(dt);
			}

			// ----------------------------------------------------- //

			SLV_WINDOW_HND.start_draw();
			
			// --------------------- DRAW HERE --------------------- //

			if (m_current_scene)
			{
				m_current_scene->base_draw();
			}

			// ----------------------------------------------------- //
			
			SLV_WINDOW_HND.end_draw();
		}

		SLV_WINDOW_HND.uninit();
	}

	void Game::change_scene(Scene* new_scene)
	{
		if (!new_scene)
		{
			slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "The scene the game tried to change into is NULL");
			return;
		}

		if (m_pending_scene)
		{
			m_pending_scene->destroy();
		}

		m_pending_scene = new_scene;
	}
}