#include <slv/game.hpp>
#include <slv/backend/handlers/window.hpp>
#include <slv/debug/log.hpp>

namespace slv
{
	Game::~Game()
	{
		slv::WindowHandler::get().uninit();
	}

	bool Game::init(std::string_view window_title, const slv::size_uint& window_size, unsigned int window_fps, bool has_vsync, bool is_resizable, bool start_fullscreen)
	{
		if (m_is_init)
		{
			return true;
		}

		if (!slv::WindowHandler::get().init(window_title, window_size, window_fps, has_vsync, is_resizable, start_fullscreen))
		{
			return false;
		}

		m_is_init = true;

		return true;
	}

	void Game::run()
	{
		slv::WindowHandler& win_handler = slv::WindowHandler::get();

		while (!win_handler.should_close())
		{
			win_handler.update();
			// -------------------- UPDATE HERE -------------------- //

			if (m_pending_scene)
			{
				m_current_scene = std::move(m_pending_scene);
				m_current_scene->m_game = this;
			}

			float dt = win_handler.get_delta_time();

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
	}

	void Game::change_scene(std::unique_ptr<slv::Scene> new_scene)
	{
		if (!new_scene)
		{
			slv::debug_log(slv::LOG_ERROR, m_class_name, "The scene the game tried to change into is NULL");
			return;
		}

		m_pending_scene = std::move(new_scene);
	}
}