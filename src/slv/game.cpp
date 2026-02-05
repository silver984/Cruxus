#include <slv/game.hpp>
#include <slv/handlers/window_handler.hpp>
#include <slv/handlers/resource_handler.hpp>
#include <slv/handlers/input_handler.hpp>
#include <slv/handlers/crash_handler.hpp>
#include <slv/core/console_log.hpp>

namespace slv
{
	Game::~Game()
	{
		slv::WindowHandler::get().uninit();
	}

	bool Game::init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps,
					bool window_has_vsync, bool is_window_resizable, bool window_starts_fullscreen, bool is_window_borderless, bool is_window_transparent)
	{
		if (m_is_init)
		{
			return true;
		}

		if (!slv::WindowHandler::get().init(window_title, window_size, fps, window_has_vsync, is_window_resizable, window_starts_fullscreen, is_window_borderless, is_window_transparent))
		{
			return false;
		}

		if (!slv::CrashHandler::get().init())
		{
			slv::console_log(slv::LOG_WARNING, M_CLASS_NAME, "SLV's crash handler failed or may not be available on this platform");
		}

		m_is_init = true;

		return true;
	}

	void Game::run()
	{
		slv::WindowHandler& win_handler = slv::WindowHandler::get();
		slv::ResourceHandler& rsrc_handler = slv::ResourceHandler::get();
		slv::InputHandler& input_handler = slv::InputHandler::get();

		while (win_handler.is_window_open())
		{
			win_handler.update();
			input_handler.update();
		
			float dt = win_handler.get_delta_time();
			
			rsrc_handler.update(dt);
			// -------------------- UPDATE HERE -------------------- //

			if (m_pending_scene)
			{
				m_current_scene = std::move(m_pending_scene);
				m_current_scene->m_game = this;
			}

			if (m_current_scene)
			{
				m_current_scene->base_update(dt);
			}

			// ----------------------------------------------------- //
			win_handler.start_draw();
			// --------------------- DRAW HERE --------------------- //

			if (m_current_scene)
			{
				m_current_scene->base_draw();
			}

			// ----------------------------------------------------- //
			win_handler.end_draw();
		}

		win_handler.uninit();
	}

	void Game::change_scene(std::unique_ptr<slv::Scene> new_scene)
	{
		if (!new_scene)
		{
			slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "The scene the game tried to change into is NULL");
			return;
		}

		m_pending_scene = std::move(new_scene);
	}
}