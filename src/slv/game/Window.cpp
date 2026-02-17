#include <slv/game/Window.hpp>
#include <slv/core/console_log.hpp>
#include <slv/core/wrappers/raylib.hpp>
#ifdef _WIN32
#include <platform/windows/console.hpp>
#include <platform/windows/memory_usage.hpp>
#include <platform/windows/layered_window.hpp>
#endif
#include <raylib.h>
#include <algorithm>
#include <cstdint>
#include <cmath>

namespace slv
{
	Window::~Window()
	{
		uninit();

#ifdef _WIN32
		if (slv::win32::is_console_open())
		{
			slv::console_log(slv::log::INFO, M_NAME, "Destroying console...");
			slv::win32::destroy_console();
		}
#endif
	}

	// private
	bool Window::init(const std::string& title, const slv::size<int>& size, int fps, int settings)
	{
		if (m_is_init)
		{
			return true;
		}

		setup_console(title);
		configure_settings(settings);

		m_default_size.width = std::max(M_LOWEST_SIZE_PX, size.width);
		m_default_size.height = std::max(M_LOWEST_SIZE_PX, size.height);
		InitWindow(m_default_size.width, m_default_size.height, title.c_str());

		if (!IsWindowReady() || !GetWindowHandle())
		{
			slv::console_log(slv::log::ERROR, M_NAME, "Failed to initialize window");
			m_default_size = slv::size<int>(0, 0);
			return false;
		}
	
		// set the minimum and maximum size of the window
		reset_minimum_size();

		int clamped_fps = std::max(1, fps);
		m_target_fps = clamped_fps;
		m_title = title;
		SetTargetFPS(m_target_fps);
		SetExitKey(KEY_NULL); // disable closing the window when ESC is pressed

		// setup transparency
		if (m_is_transparent)
		{
			create_buffers();
			slv::win32::init_layered_window(GetWindowHandle());
		}

		m_is_init = true;
		update();

		slv::console_log(slv::log::INFO, M_NAME, "Window initialized");

		return true;
	}

	// private
	void Window::uninit()
	{
		if (!m_is_init)
		{
			return;
		}

		slv::console_log(slv::log::INFO, M_NAME, "Destroying window...");

		slv::raylib::unload_render_texture(m_view);

		if (m_is_transparent)
		{
#ifdef _WIN32
			slv::win32::cleanup_layered_window();
#endif
		}

		m_is_init = false;

		CloseWindow();
	}
	
	// private
	void Window::update()
	{
		if (!m_is_init)
		{
			return;
		}

		m_current_size = slv::size<int>(GetRenderWidth(), GetRenderHeight());

		if (m_is_transparent && m_current_size != m_last_current_size)
		{
			create_buffers();
			m_last_current_size = m_current_size;
		}

		m_pos = slv::vec2<int>(static_cast<int>(GetWindowPosition().x), static_cast<int>(GetWindowPosition().y));

#ifdef _WIN32
		// windows automatically fullscreens the window when the size is the same as the monitor's and if the window position is (0, 0)
		// this disables that in case if the window is not supposed to be fullscreen
		if (!m_is_fullscreen && m_pos == slv::vec2<int>(0, 0) && m_current_size == monitor_size())
		{
			set_pos(slv::vec2<int>(0, -1));
		}
#endif

		// update fullscreen
		// transparent windows cannot be fullscreen
		if (!m_is_transparent && IsKeyPressed(KEY_F11))
		{
			if (!IsWindowFullscreen()) // going fullscreen
			{
				m_unmaximized_size = m_current_size;
				auto _monitor_size = monitor_size();
				SetWindowSize(_monitor_size.width, _monitor_size.height);
				m_is_fullscreen = true;
			}
			else // leaving fullscreen
			{
				SetWindowSize(m_unmaximized_size.width, m_unmaximized_size.height);
				m_is_fullscreen = false;
			}
		
			ToggleFullscreen();
		}
	}

	// private
	void Window::start_draw() const
	{
		if (!m_is_init)
		{
			return;
		}

		BeginDrawing();
		BeginScissorMode(0, 0, m_current_size.width, m_current_size.height);

		if (m_is_transparent)
		{
			slv::raylib::begin_texture_mode(m_view);
			ClearBackground(BLANK);
			return;
		}

		ClearBackground(BLACK);
	}

	// private
	void Window::end_draw()
	{
		if (!m_is_init)
		{
			return;
		}

#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
		int text_size = 10;
		int text_padding = text_size;
		int text_border_padding = 5;
		DrawText(fmt::format("FPS: {}", running_fps()).c_str(), text_border_padding, text_border_padding, text_size, WHITE);

		float memory_usage = 0.0F;
#ifdef _WIN32
		memory_usage = slv::win32::get_memory_mb();
#endif
		if (memory_usage != 0.0F)
		{
			DrawText(fmt::format("MEM: {:.2f}MB", memory_usage).c_str(), text_border_padding, text_border_padding + text_padding, text_size, WHITE);
		}
#endif

		if (m_is_transparent)
		{
			EndTextureMode();
			Image img = LoadImageFromTexture(Texture(m_view.tex.id, m_view.tex.width, m_view.tex.height, m_view.tex.mipmaps, m_view.tex.format));
#ifdef _WIN32
			std::memcpy(m_render_buffers.rgba.data(), img.data, static_cast<size_t>(img.width) * img.height * 4);
#endif
			UnloadImage(img);
#ifdef _WIN32
			slv::win32::convert_rgba_to_bgra(m_render_buffers);
			slv::win32::update_layered_window(m_render_buffers);
#endif
		}
		
		EndScissorMode();
		EndDrawing();
	}

	bool Window::is_open() const
	{
		if (m_is_init)
		{
			return !WindowShouldClose();
		}

		return false;
	}

	slv::vec2<float> Window::screen_center() const
	{
		float _ui_scale = ui_scale();
		return slv::vec2<float>((m_current_size.width / 2.0F) / _ui_scale,
								 (m_current_size.height / 2.0F) / _ui_scale);
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
		if (!m_is_init)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_size.width = std::max(M_LOWEST_SIZE_PX, width);
		}

		reset_minimum_size(); // reset minimum size of the window
		SetWindowSize(set_as_default ? m_default_size.width : std::max(M_LOWEST_SIZE_PX, width), m_default_size.height);
	}

	void Window::set_height(int height, bool set_as_default)
	{
		if (!m_is_init)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_size.height = std::max(M_LOWEST_SIZE_PX, height);
		}

		reset_minimum_size(); // reset minimum size of the window
		SetWindowSize(m_default_size.width, set_as_default ? m_default_size.height : std::max(M_LOWEST_SIZE_PX, height));
	}

	void Window::set_title(const std::string& title)
	{
		if (m_is_init)
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
		if (m_is_init)
		{
			m_target_fps = std::max(1, fps);
			SetTargetFPS(m_target_fps);
		}
	}

	slv::size<int> Window::monitor_size() const
	{
		if (m_is_init)
		{
			int monitor = GetCurrentMonitor();
			return slv::size<int>(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		}

		return slv::size<int>();
	}

	float Window::delta_time() const
	{
		if (m_is_init)
		{
			return GetFrameTime();
		}

		return 0.0F;
	}

	slv::vec2<float> Window::mouse_pos() const
	{
		if (m_is_init)
		{
			Vector2 pos = GetMousePosition();
			float _ui_scale = ui_scale();
			return slv::vec2<float>(pos.x / _ui_scale, pos.y / _ui_scale);
		}

		return slv::vec2<float>();
	}

	slv::vec2<float> Window::mouse_delta() const
	{
		if (m_is_init)
		{
			Vector2 dt = GetMouseDelta();
			return { dt.x, dt.y };
		}

		return slv::vec2<float>();
	}

	float Window::ui_scale() const
	{
		float w = m_default_size.width > 0 ? static_cast<float>(m_current_size.width) / m_default_size.width : 1.0F;
		float h = m_default_size.height > 0 ? static_cast<float>(m_current_size.height) / m_default_size.height : 1.0F;
		return std::min(w, h);
	}

	int Window::running_fps() const
	{
		float dt = delta_time();
		if (dt != 0.0F)
		{
			return static_cast<int>(round(1.0F / dt));
		}

		return 0;
	}

	void Window::set_pos(const slv::vec2<int>& pos)
	{
		if (m_is_init && !IsWindowFullscreen())
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

	bool Window::is_fullscreen() const
	{
		if (m_is_init)
		{
			return IsWindowFullscreen();
		}

		return false;
	}

	// private
	void Window::setup_console(const std::string& window_title)
	{
		// disable raylib's logs
		SetTraceLogCallback([](int, const char*, va_list){});

#if (defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO))
#ifdef _WIN32
		if (slv::win32::create_console(window_title))
		{
			slv::console_log(slv::log::INFO, M_NAME, "Console initialized");
		}
#else
		slv::console_log(slv::log::WARNING, M_NAME, "SLV's debug console is not supported on this platform");
#endif
#endif
	}

	// private
	void Window::configure_settings(int settings)
	{
		using enum slv::window_settings;
		bool vsync = (settings & VSYNC) != 0;
		bool resizable = (settings & RESIZABLE) != 0;
		bool start_fullscreen = (settings & START_FULLSCREEN) != 0;
		bool borderless = (settings & BORDERLESS) != 0;
		bool transparent = (settings & TRANSPARENT) != 0;

#ifdef _WIN32
		if (transparent)
		{
			resizable = false;
			start_fullscreen = false; // transparent windows cannot be fullscreen
			borderless = true;
		}
#else
		transparent = false;
		slv::console_log(slv::log::WARNING, M_NAME, "SLV's transparent window feature is not supported on this platform");
#endif

		int flags = 0;

		if (vsync)
		{
			flags |= FLAG_VSYNC_HINT;
		}

		if (borderless)
		{
			resizable = false;
			flags |= FLAG_WINDOW_UNDECORATED;
		}

		if (resizable)
		{
			flags |= FLAG_WINDOW_RESIZABLE;
		}

		if (start_fullscreen)
		{
			flags |= FLAG_FULLSCREEN_MODE;
		}

		SetConfigFlags(flags);

		m_is_fullscreen = start_fullscreen;
		m_is_transparent = transparent;
	}

	// private
	void Window::create_buffers()
	{
		if (m_view.id != 0)
		{
			slv::raylib::unload_render_texture(m_view);
		}

		m_view = slv::raylib::load_render_texture(m_current_size.width, m_current_size.height);
#ifdef _WIN32
		m_render_buffers = slv::win32::create_render_buffers(m_current_size.width, m_current_size.height);
#endif
	}

	// private
	void Window::reset_minimum_size()
	{
		m_minimum_size.width = std::min(m_default_size.width, M_LOW_SIZE.width);
		m_minimum_size.height = std::min(m_default_size.height, M_LOW_SIZE.height);
		SetWindowMinSize(m_minimum_size.width, m_minimum_size.height);
	}
}