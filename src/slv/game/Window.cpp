#include <slv/game/Window.hpp>
#include <fmt/format.h>
#include <slv/core/console/log.hpp>
#include <slv/core/wrappers/raylib.hpp>
#ifdef _WIN32
#include <platform/windows/console.hpp>
#include <platform/windows/memory_usage.hpp>
#endif
#include <raylib.h>
#include <algorithm>
#include <cstdint>
#include <cmath>

namespace slv
{
	// private
	Window::~Window()
	{
		uninit();
	}

	// private
	bool Window::init(const std::string& title, const slv::size<int>& size, int fps, int settings)
	{
		if (m_is_initialized)
		{
			return true;
		}
		
		// disable raylib's logs
		SetTraceLogCallback([](int, const char*, va_list) {});
		m_title = title;
#if (defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)) && defined(_WIN32)
		open_console();
#endif
		
		configure_settings(settings);

		m_default_size.width = std::max(M_LOWEST_SIZE_PX, size.width);
		m_default_size.height = std::max(M_LOWEST_SIZE_PX, size.height);
		InitWindow(m_default_size.width, m_default_size.height, title.c_str());

		if (!IsWindowReady() || !GetWindowHandle())
		{
			slv::log::error(M_NAME, "Failed to initialize window");
			m_default_size = slv::size<int>(0, 0);
			m_title = "";
			return false;
		}
	
		// set the minimum and maximum size of the window
		reset_minimum_size();

		m_target_fps = std::max(1, fps);
		SetTargetFPS(m_target_fps);
		SetExitKey(KEY_NULL); // disable closing the window when ESC is pressed

		m_is_initialized = true;
		update(0.f);
		slv::log::info(M_NAME, "Window initialized");

		return true;
	}

	// private
	void Window::uninit()
	{
		if (!m_is_initialized)
		{
			return;
		}

		m_is_initialized = false;
		
		slv::log::info(M_NAME, "Destroying window...");

		CloseWindow();

#ifdef _WIN32
		close_console();
#endif
	}
	
	// private
	void Window::update(float dt)
	{
		if (!m_is_initialized)
		{
			return;
		}

		m_current_size = slv::size<int>(GetRenderWidth(), GetRenderHeight());

		if (m_last_current_size != m_current_size)
		{
			m_was_resized = true;
			m_last_current_size = m_current_size;
		}
		else
		{
			m_was_resized = false;
		}

		m_pos = slv::vec2<int>(static_cast<int>(GetWindowPosition().x), static_cast<int>(GetWindowPosition().y));

#ifdef _WIN32
		// it seems that on windows,
		// a window automatically fullscreens the window when the size is the same as the monitor's and if the window position is (0, 0)
		// this disables that in case if the window is not supposed to be fullscreen
		if (!is_fullscreen() && m_pos == slv::vec2<int>(0, 0) && m_current_size == monitor_size())
		{
			set_pos(slv::vec2<int>(0, -1));
		}
#endif

		if (IsKeyPressed(KEY_F11))
		{
			m_invoked_fullscreen = !m_invoked_fullscreen;
			toggle_fullscreen(m_invoked_fullscreen);
		}

		m_elapsed += dt;
		m_frame_count++;

		while (m_elapsed >= 1.0f)
		{
			m_running_fps = m_frame_count;
			m_frame_count = 0;
			m_elapsed -= 1.0f;
		}
	}

	// private
	void Window::start_draw() const
	{
		if (!m_is_initialized)
		{
			return;
		}

		BeginDrawing();
		BeginScissorMode(0, 0, m_current_size.width, m_current_size.height);

		if (is_transparent())
		{
			ClearBackground(BLANK);
			return;
		}

		ClearBackground(BLACK);
	}

	// private
	void Window::end_draw() const
	{
		if (!m_is_initialized)
		{
			return;
		}

#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
		int text_size = 10;
		int text_padding = text_size;
		int text_border_padding = 5;
		DrawText(fmt::format("FPS: {}", running_fps()).c_str(), text_border_padding, text_border_padding, text_size, WHITE);

		float memory_usage = 0.f;
#ifdef _WIN32
		memory_usage = slv::win32::get_memory_mb();
#endif
		if (memory_usage != 0.f)
		{
			DrawText(fmt::format("MEM: {:.2f}MB", memory_usage).c_str(), text_border_padding, text_border_padding + text_padding, text_size, WHITE);
		}
#endif
		
		EndScissorMode();
		EndDrawing();
	}

	bool Window::is_open() const
	{
		if (m_is_initialized)
		{
			return !WindowShouldClose();
		}

		return false;
	}

	slv::vec2<float> Window::screen_center() const
	{
		float _ui_scale = ui_scale();
		return slv::vec2<float>((m_current_size.width / 2.f) / _ui_scale,
								(m_current_size.height / 2.f) / _ui_scale);
	}

	slv::size<float> Window::screen_size() const
	{
		float _ui_scale = ui_scale();
		return slv::size<float>(m_current_size.width / _ui_scale,
								m_current_size.height / _ui_scale);
	}

	// TODO: fix window sizing

	void Window::set_size(const slv::size<int>& size, bool set_as_default)
	{
		set_width(size.width, set_as_default);
		set_height(size.height, set_as_default);
	}

	void Window::set_width(int width, bool set_as_default)
	{
		if (!m_is_initialized)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_size.width = std::max(M_LOWEST_SIZE_PX, width);
		}

		reset_minimum_size(); // reset minimum size of the window
		SetWindowSize(set_as_default ? m_default_size.width : std::max(M_LOWEST_SIZE_PX, width), m_default_size.height);
		m_was_resized = true;
	}

	void Window::set_height(int height, bool set_as_default)
	{
		if (!m_is_initialized)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_size.height = std::max(M_LOWEST_SIZE_PX, height);
		}

		reset_minimum_size(); // reset minimum size of the window
		SetWindowSize(m_default_size.width, set_as_default ? m_default_size.height : std::max(M_LOWEST_SIZE_PX, height));
		m_was_resized = true;
	}

	void Window::set_title(const std::string& title)
	{
		if (m_is_initialized)
		{
			m_title = title;
			SetWindowTitle(m_title.c_str());
			
#ifdef _WIN32
			if (slv::win32::is_console_open())
			{
				slv::win32::rename_console(m_title);
			}
#endif
		}
	}

	void Window::set_fps(int fps)
	{
		if (m_is_initialized)
		{
			m_target_fps = std::max(1, fps);
			SetTargetFPS(m_target_fps);
		}
	}

	slv::size<int> Window::monitor_size() const
	{
		if (m_is_initialized)
		{
			int monitor = GetCurrentMonitor();
			return slv::size<int>(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		}

		return slv::size<int>();
	}

	float Window::delta_time() const
	{
		if (m_is_initialized)
		{
			return GetFrameTime();
		}

		return 0.f;
	}

	slv::vec2<float> Window::mouse_pos() const
	{
		if (m_is_initialized)
		{
			Vector2 pos = GetMousePosition();
			float _ui_scale = ui_scale();
			return _ui_scale != 0.f ? slv::vec2<float>(pos.x / _ui_scale, pos.y / _ui_scale) : slv::vec2<float>(pos.x, pos.y);
		}

		return slv::vec2<float>();
	}

	slv::vec2<float> Window::mouse_delta() const
	{
		if (m_is_initialized)
		{
			Vector2 dt = GetMouseDelta();
			return slv::vec2<float>(dt.x, dt.y);
		}

		return slv::vec2<float>();
	}

	float Window::ui_scale() const
	{
		float w = m_default_size.width > 0 ? static_cast<float>(m_current_size.width) / m_default_size.width : 1.f;
		float h = m_default_size.height > 0 ? static_cast<float>(m_current_size.height) / m_default_size.height : 1.f;
		return std::min(w, h);
	}

	void Window::set_pos(const slv::vec2<int>& pos)
	{
		if (m_is_initialized && !IsWindowFullscreen())
		{
			m_pos = pos;
			SetWindowPosition(pos.x, pos.y);
		}
	}

	void Window::set_posx(int x)
	{
		set_pos(slv::vec2<int>(x, m_pos.y));
	}

	void Window::set_posy(int y)
	{
		set_pos(slv::vec2<int>(m_pos.x, y));
	}

	void Window::toggle_fullscreen(bool val)
	{
		if (is_transparent())
		{
			return;
		}

		if (val && !IsWindowFullscreen())
		{
			m_unmaximized_size = m_current_size;
			auto _monitor_size = monitor_size();
			SetWindowSize(_monitor_size.width, _monitor_size.height);
			ToggleFullscreen();
			return;
		}

		SetWindowSize(m_unmaximized_size.width, m_unmaximized_size.height);
		ToggleFullscreen();
	}

	bool Window::is_fullscreen() const
	{
		if (m_is_initialized)
		{
			return IsWindowFullscreen();
		}

		return false;
	}

	bool Window::is_transparent() const
	{
		if (m_is_initialized)
		{
			return IsWindowState(FLAG_WINDOW_TRANSPARENT);
		}

		return false;
	}

	bool Window::is_resizable() const
	{
		if (m_is_initialized)
		{
			return IsWindowState(FLAG_WINDOW_RESIZABLE);
		}

		return false;
	}

	bool Window::is_borderless() const
	{
		if (m_is_initialized)
		{
			return IsWindowState(FLAG_WINDOW_UNDECORATED);
		}

		return false;
	}

	bool Window::has_vsync() const
	{
		if (m_is_initialized)
		{
			return IsWindowState(FLAG_VSYNC_HINT);
		}

		return false;
	}

#ifdef _WIN32
	void Window::open_console() const
	{
		if (slv::win32::create_console(m_title))
		{
			slv::log::info(M_NAME, "Windows console initialized");
		}
	}

	void Window::close_console() const
	{
		if (slv::win32::is_console_open())
		{
			slv::log::info(M_NAME, "Destroying console...");
			slv::win32::destroy_console();
		}
	}
#endif

	// private
	void Window::configure_settings(int settings)
	{
		using enum slv::window_settings;
		bool vsync = (settings & VSYNC) != NONE;
		bool unresizable = (settings & UNRESIZABLE) != NONE;
		bool start_fullscreen = (settings & START_FULLSCREEN) != NONE; // TODO: fix startup with this setting on
		bool borderless = (settings & BORDERLESS) != NONE;
		bool transparent = (settings & TRANSPARENT) != NONE;

		int flags = 0;
		
		if (transparent)
		{
			flags |= FLAG_WINDOW_TRANSPARENT;
			start_fullscreen = false;
			borderless = true;
		}

		if (vsync)
		{
			flags |= FLAG_VSYNC_HINT;
		}

		if (borderless)
		{
			unresizable = true;
			flags |= FLAG_WINDOW_UNDECORATED;
		}

		bool resizable = !unresizable; // clarity
		if (resizable)
		{
			flags |= FLAG_WINDOW_RESIZABLE;
		}

		if (start_fullscreen)
		{
			flags |= FLAG_FULLSCREEN_MODE;
		}

		SetConfigFlags(flags);
	}

	// private
	void Window::reset_minimum_size()
	{
		m_minimum_size.width = std::min(m_default_size.width, M_LOW_SIZE.width);
		m_minimum_size.height = std::min(m_default_size.height, M_LOW_SIZE.height);
		SetWindowMinSize(m_minimum_size.width, m_minimum_size.height);
	}
}