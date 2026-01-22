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

		SetTraceLogCallback([](int, const char*, va_list) {});

#if defined(SLV_DEBUG) && defined(_WIN32)
		if (slv::win32::create_console(window_title))
		{
			slv::debug_log(slv::LOG_INFO, m_class_name, "Console initialized");
		}
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

		InitWindow(m_original_win_size.width, m_original_win_size.height, std::string(m_win_title).c_str());

		if (!IsWindowReady())
		{
			slv::debug_log(slv::LOG_ERROR, m_class_name, "Failed to initialize window");
			return false;
		}

		SetWindowMinSize(800, 600);
		SetWindowMaxSize(get_monitor_size().width, get_monitor_size().height);
		
		SetTargetFPS(m_fps);
		
		SetExitKey(KEY_NULL);

		m_is_init = true;

		slv::debug_log(slv::LOG_INFO, m_class_name, "Window initialized");

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
			static_cast<unsigned int>(GetScreenWidth()),
			static_cast<unsigned int>(GetScreenHeight())
		};

		if (IsKeyPressed(KEY_F11))
		{
			if (!IsWindowFullscreen()) // going fullscreen
			{
				m_last_win_size = m_current_win_size;
				SetWindowSize(get_monitor_size().width, get_monitor_size().height);
			}
			else // leaving fullscreen
			{
				SetWindowSize(m_last_win_size.width, m_last_win_size.height);
			}
		
			ToggleFullscreen();
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

	bool WindowHandler::should_close() const
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

	const slv::size_uint WindowHandler::get_monitor_size() const
	{
		if (m_is_init)
		{
			int monitor = GetCurrentMonitor();

			return slv::size_uint
			{
				static_cast<unsigned int>(GetMonitorWidth(monitor)),
				static_cast<unsigned int>(GetMonitorHeight(monitor)),
			};
		}

		return slv::size_uint{};
	}
}