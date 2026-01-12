#include <slv/backend/handlers/window.hpp>
#include <slv/debug/log.hpp>
#include <raylib.h>

namespace slv
{
	bool WindowHandler::init(std::string_view window_title, const slv::size_uint& window_size, unsigned int fps, bool has_vsync, bool is_resizable, bool start_fullscreen)
	{
		if (m_is_init)
		{
			return true;
		}

#if defined(SLV_DEBUG) && defined(_WIN32)
		slv::win32::create_console(window_title);
#endif

		int flags = 0;

		if (has_vsync)
		{
			flags |= FLAG_VSYNC_HINT;
		}

		if (is_resizable)
		{
			flags |= FLAG_WINDOW_RESIZABLE;
		}

		if (start_fullscreen)
		{
			flags |= FLAG_FULLSCREEN_MODE;
		}

		SetConfigFlags(flags);

		m_original_win_size = window_size;
		m_win_title = window_title;
		m_fps = fps;

		SetTargetFPS(m_fps);
		InitWindow(m_original_win_size.width, m_original_win_size.height, std::string(m_win_title).c_str());
		SetExitKey(KEY_NULL);

		m_is_init = true;

		slv::console_log(slv::LVL_INFO, "Window initialized", m_class_name);

		return true;
	}

	void WindowHandler::uninit()
	{
		if (!m_is_init)
		{
			return;
		}

		m_is_init = false;
		CloseWindow();
	}
	
	void WindowHandler::update()
	{
		if (!m_is_init)
		{
			return;
		}

		m_current_win_size = slv::size_uint
		{
			static_cast<unsigned int>(GetRenderWidth()),
			static_cast<unsigned int>(GetRenderHeight())
		};

		if (IsWindowResized())
		{
			slv::console_log(slv::LVL_INFO, "Window resized", m_class_name);
			on_resized();
		}
	}

	void WindowHandler::start_draw() const
	{
		if (!m_is_init)
		{
			return;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		BeginScissorMode(0, 0, m_current_win_size.width, m_current_win_size.height);
#ifdef SLV_DEBUG
		DrawFPS(5, 5);
#endif
	}

	void WindowHandler::end_draw() const
	{
		if (m_is_init)
		{
			EndScissorMode();
			EndDrawing();
		}
	}

	void WindowHandler::on_resized() const
	{
		if (!m_is_init)
		{
			return;
		}

		float ideal_ratio = static_cast<float>(m_original_win_size.width) / m_original_win_size.height;
		float current_ratio = static_cast<float>(m_current_win_size.width) / m_current_win_size.height;

		slv::size_uint win_size{};

		if (current_ratio > ideal_ratio)
		{
			slv::console_log(slv::LVL_INFO, "1", m_class_name);
			win_size.height = m_current_win_size.height;
			win_size.width = static_cast<int>(win_size.height * ideal_ratio);
		}
		else
		{
			slv::console_log(slv::LVL_INFO, "2", m_class_name);
			win_size.width = m_current_win_size.width;
			win_size.height = static_cast<int>(win_size.width / ideal_ratio);
		}

		SetWindowSize(win_size.width, win_size.height);
	}

	bool WindowHandler::should_close()
	{
		return WindowShouldClose();
	}

	void WindowHandler::set_window_size(const slv::size_uint& size)
	{
		if (m_is_init)
		{
			m_original_win_size = size;
			SetWindowSize(m_original_win_size.width, m_original_win_size.height);
		}
	}

	void WindowHandler::set_window_title(std::string_view title)
	{
		if (m_is_init)
		{
			m_win_title = title;
			SetWindowTitle(m_win_title.c_str());
		}
	}

	void WindowHandler::set_fps(unsigned int fps)
	{
		if (m_is_init)
		{
			m_fps = fps;
			SetTargetFPS(m_fps);
		}
	}
}