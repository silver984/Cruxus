#include <slv/backend/handlers/window.hpp>
#include <slv/debug/log.hpp>
#include <slv/backend/debug/console/data.hpp>
#ifdef _WIN32
#include <slv/backend/debug/console/windows.hpp>
#include <slv/backend/debug/memory/windows.hpp>
#endif
#include <raylib.h>
#include <cmath>

namespace slv
{
	WindowHandler::~WindowHandler()
	{
		if (slv::is_console_active)
		{
			slv::debug_log(slv::LOG_INFO, M_CLASS_NAME, "Destroying console...");
#ifdef _WIN32
			slv::win32::destroy_console();
#endif
		}
	}

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
			slv::debug_log(slv::LOG_INFO, M_CLASS_NAME, "Console initialized");
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
			slv::debug_log(slv::LOG_ERROR, M_CLASS_NAME, "Failed to initialize window");
			return false;
		}

		SetWindowMinSize(800, 600);
		SetWindowMaxSize(get_monitor_size().width, get_monitor_size().height);
		
		SetTargetFPS(m_fps);
		
		SetExitKey(KEY_NULL);

		m_is_init = true;

		slv::debug_log(slv::LOG_INFO, M_CLASS_NAME, "Window initialized");

		return true;
	}

	void WindowHandler::uninit()
	{
		if (!m_is_init)
		{
			return;
		}

		slv::debug_log(slv::LOG_INFO, M_CLASS_NAME, "Destroying window...");
		
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
	}

	void WindowHandler::end_draw() const
	{
		if (!m_is_init)
		{
			return;
		}

#ifdef SLV_DEBUG
		int fps = static_cast<int>(ceil(1.f / get_delta_time()));
		float memory_usage = 0.f;
#ifdef _WIN32
		memory_usage = slv::win32::get_memory_mb();
#endif

		int text_size = 10;
		int text_padding = text_size / 2;
		int text_border_padding = 5;
		DrawText(fmt::format("FPS: {}", fps).c_str(), text_border_padding, text_border_padding, text_size, WHITE);

		if (memory_usage != 0.f)
		{
			DrawText(fmt::format("MEM: {:.2f}MB", memory_usage).c_str(), text_border_padding, text_border_padding + (text_padding * 2), text_size, WHITE);
		}
#endif
		EndScissorMode();
		EndDrawing();
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
			
			if (slv::is_console_active)
			{
				slv::win32::rename_console(m_win_title);
			}
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

	const float WindowHandler::get_delta_time() const
	{
		return GetFrameTime();
	}

	const slv::vec_2 WindowHandler::get_mouse_pos() const
	{
		Vector2 pos = GetMousePosition();
		float ui_scale = get_ui_scale();
		return slv::vec_2{ pos.x / ui_scale, pos.y / ui_scale };
	}

	const slv::vec_2 WindowHandler::get_mouse_delta() const
	{
		Vector2 dt = GetMouseDelta();
		return slv::vec_2{ dt.x, dt.y };
	}

	float WindowHandler::get_ui_scale() const
	{
		float ratio_width = static_cast<float>(m_current_win_size.width) / m_original_win_size.width;
		float ratio_height = static_cast<float>(m_current_win_size.height) / m_original_win_size.height;
		return std::min(ratio_width, ratio_height);
	}
}