#include <slv/handlers/window_handler.hpp>
#include <slv/core/console_log.hpp>
#include <slv/core/wrappers/raylib.hpp>
#ifdef _WIN32
#include <platform/windows/console.hpp>
#include <platform/windows/memory_usage.hpp>
#include <platform/windows/layered_window.hpp>
#endif
#include <raylib.h>
#include <algorithm>

namespace slv
{
	WindowHandler::~WindowHandler()
	{
#ifdef _WIN32
		if (slv::win32::is_console_open())
		{
			slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Destroying console...");
			slv::win32::destroy_console();
		}
#endif
	}

	bool WindowHandler::init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps,
							 bool window_has_vsync, bool is_window_resizable, bool window_starts_fullscreen, bool is_window_borderless, bool is_window_transparent)
	{
		if (m_is_init)
		{
			return true;
		}

		setup_console(window_title);
		configure_flags(window_has_vsync, is_window_resizable, window_starts_fullscreen, is_window_borderless, is_window_transparent);

		m_default_window_size.width = std::max(M_LOWEST_WINDOW_SIZE_PX, window_size.width);
		m_default_window_size.height = std::max(M_LOWEST_WINDOW_SIZE_PX, window_size.height);
		InitWindow(m_default_window_size.width, m_default_window_size.height, window_title.c_str());

		if (!IsWindowReady() || !GetWindowHandle())
		{
			slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "Failed to initialize window");
			m_default_window_size = slv::size<unsigned int>();
			return false;
		}
	
		// set the minimum and maximum size of the window
		reset_minimum_window_size();
		SetExitKey(KEY_NULL); // disable closing the window when ESC is pressed
		SetTargetFPS(fps);

		m_target_fps = fps;
		m_window_title = window_title;
		m_is_window_fullscreen = window_starts_fullscreen;
		m_is_window_transparent = is_window_transparent;
		
		// setup transparency
		if (m_is_window_transparent)
		{
			create_buffers();
			slv::win32::init_layered_window(GetWindowHandle());
		}

		m_is_init = true;
		update();

		slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Window initialized");

		return true;
	}

	void WindowHandler::uninit()
	{
		if (!m_is_init)
		{
			return;
		}

		slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Destroying window...");

		slv::raylib::unload_render_texture(m_view);

		if (m_is_window_transparent)
		{
#ifdef _WIN32
			slv::win32::cleanup_layered_window();
#endif
		}

		m_is_init = false;

		CloseWindow();
	}
	
	// private
	void WindowHandler::update()
	{
		if (!m_is_init)
		{
			return;
		}

		m_current_window_size = slv::size<unsigned int>(static_cast<unsigned int>(GetRenderWidth()), static_cast<unsigned int>(GetRenderHeight()));

		if (m_is_window_transparent && m_current_window_size != m_last_current_window_size)
		{
			create_buffers();
			m_last_current_window_size = m_current_window_size;
		}

		m_window_pos = slv::vec_2<int>(static_cast<int>(GetWindowPosition().x), static_cast<int>(GetWindowPosition().y));

#ifdef _WIN32
		// windows automatically fullscreens the window when the size is the same as the monitor's and if the window position is (0, 0)
		// this disables that in case if the window is not supposed to be fullscreen
		if (!m_is_window_fullscreen && m_window_pos == slv::vec_2<int>(0, 0) && m_current_window_size == get_monitor_size())
		{
			set_window_pos(slv::vec_2<int>(0, -1));
		}
#endif

		// update fullscreen
		// transparent windows cannot be fullscreen
		if (!m_is_window_transparent && IsKeyPressed(KEY_F11))
		{
			if (!IsWindowFullscreen()) // going fullscreen
			{
				m_unmaximized_window_size = m_current_window_size;
				SetWindowSize(get_monitor_size().width, get_monitor_size().height);
				m_is_window_fullscreen = true;
			}
			else // leaving fullscreen
			{
				SetWindowSize(m_unmaximized_window_size.width, m_unmaximized_window_size.height);
				m_is_window_fullscreen = false;
			}
		
			ToggleFullscreen();
		}
	}

	// private
	void WindowHandler::start_draw() const
	{
		if (!m_is_init)
		{
			return;
		}

		BeginDrawing();
		BeginScissorMode(0, 0, m_current_window_size.width, m_current_window_size.height);

		if (m_is_window_transparent)
		{
			slv::raylib::begin_texture_mode(m_view);
			ClearBackground(BLANK);
			return;
		}

		ClearBackground(BLACK);
	}

	// private
	void WindowHandler::end_draw()
	{
		if (!m_is_init)
		{
			return;
		}

#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
		int text_size = 10;
		int text_padding = text_size / 2;
		int text_border_padding = 5;
		DrawText(fmt::format("FPS: {}", get_running_fps()).c_str(), text_border_padding, text_border_padding, text_size, WHITE);

		float memory_usage = 0.0F;
#ifdef _WIN32
		memory_usage = slv::win32::get_memory_mb();
#endif
		if (memory_usage != 0.0F)
		{
			DrawText(fmt::format("MEM: {:.2f}MB", memory_usage).c_str(), text_border_padding, text_border_padding + (text_padding * 2), text_size, WHITE);
		}
#endif

		if (m_is_window_transparent)
		{
			EndTextureMode();
			Image img = LoadImageFromTexture(Texture(m_view.tex.id, m_view.tex.width, m_view.tex.height, m_view.tex.mipmaps, m_view.tex.format));
			std::memcpy(m_render_buffers.rgba.data(), img.data, static_cast<size_t>(img.width) * img.height * 4);
			UnloadImage(img);
#ifdef _WIN32
			slv::win32::convert_rgba_to_bgra(m_render_buffers);
			slv::win32::update_layered_window(m_render_buffers);
#endif
		}
		
		EndScissorMode();
		EndDrawing();
	}

	bool WindowHandler::is_window_open() const
	{
		if (m_is_init)
		{
			return !WindowShouldClose();
		}

		return false;
	}

	void WindowHandler::set_window_size(const slv::size<unsigned int>& size, bool set_as_default)
	{
		set_window_width(size.width, set_as_default);
		set_window_height(size.height, set_as_default);
	}

	void WindowHandler::set_window_width(unsigned int width, bool set_as_default)
	{
		if (!m_is_init)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_window_size.width = std::max(M_LOWEST_WINDOW_SIZE_PX, width);
		}
		else
		{
			width = std::max(M_LOWEST_WINDOW_SIZE_PX, width);
		}

		reset_minimum_window_size(); // reset minimum size of the window

		SetWindowSize(set_as_default ? m_default_window_size.width : width, m_default_window_size.height);
	}

	void WindowHandler::set_window_height(unsigned int height, bool set_as_default)
	{
		if (!m_is_init)
		{
			return;
		}

		if (set_as_default)
		{
			m_default_window_size.height = std::max(M_LOWEST_WINDOW_SIZE_PX, height);
		}
		else
		{
			height = std::max(M_LOWEST_WINDOW_SIZE_PX, height);
		}

		reset_minimum_window_size(); // reset minimum size of the window

		SetWindowSize(m_default_window_size.width, set_as_default ? m_default_window_size.height : height);
	}

	void WindowHandler::set_window_title(const std::string& title)
	{
		if (m_is_init)
		{
			m_window_title = title;
			SetWindowTitle(m_window_title.c_str());
			
#ifdef _WIN32
			if (slv::win32::is_console_open())
			{
				slv::win32::rename_console(m_window_title);
			}
#endif
		}
	}

	void WindowHandler::set_fps(unsigned int fps)
	{
		if (m_is_init)
		{
			m_target_fps = fps;
			SetTargetFPS(m_target_fps);
		}
	}

	slv::size<unsigned int> WindowHandler::get_monitor_size() const
	{
		if (m_is_init)
		{
			int monitor = GetCurrentMonitor();
			return slv::size<unsigned int>(static_cast<unsigned int>(GetMonitorWidth(monitor)),
										   static_cast<unsigned int>(GetMonitorHeight(monitor)));
		}

		return slv::size<unsigned int>();
	}

	float WindowHandler::get_delta_time() const
	{
		if (m_is_init)
		{
			return GetFrameTime();
		}

		return 0.0F;
	}

	slv::vec_2<float> WindowHandler::get_mouse_pos() const
	{
		if (m_is_init)
		{
			Vector2 pos = GetMousePosition();
			float ui_scale = get_ui_scale();
			return { pos.x / ui_scale, pos.y / ui_scale };
		}

		return slv::vec_2<float>();
	}

	slv::vec_2<float> WindowHandler::get_mouse_delta() const
	{
		if (m_is_init)
		{
			Vector2 dt = GetMouseDelta();
			return { dt.x, dt.y };
		}

		return slv::vec_2<float>();
	}

	float WindowHandler::get_ui_scale() const
	{
		float w = m_default_window_size.width > 0U ? static_cast<float>(m_current_window_size.width) / m_default_window_size.width : 1.0F;
		float h = m_default_window_size.height > 0U ? static_cast<float>(m_current_window_size.height) / m_default_window_size.height : 1.0F;
		return std::min(w, h);
	}

	int WindowHandler::get_running_fps() const
	{
		float dt = get_delta_time();
		if (dt != 0.0F)
		{
			return static_cast<int>(round(1.0F / dt));
		}

		return 0;
	}

	void WindowHandler::set_window_pos(const slv::vec_2<int>& pos)
	{
		if (m_is_init && !IsWindowFullscreen())
		{
			m_window_pos = pos;
			SetWindowPosition(pos.x, pos.y);
		}
	}

	void WindowHandler::set_window_pos_x(int x)
	{
		set_window_pos(slv::vec_2<int>(x, m_window_pos.y));
	}

	void WindowHandler::set_window_pos_y(int y)
	{
		set_window_pos(slv::vec_2<int>(m_window_pos.x, y));
	}

	bool WindowHandler::is_window_fullscreen() const
	{
		if (m_is_init)
		{
			return IsWindowFullscreen();
		}

		return false;
	}

	// private
	void WindowHandler::setup_console(const std::string& window_title)
	{
		// disable raylib's console logs
		SetTraceLogCallback([](int, const char*, va_list){});

#if (defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO))
#ifdef _WIN32
		if (slv::win32::create_console(window_title))
		{
			slv::console_log(slv::LOG_INFO, M_CLASS_NAME, "Console initialized");
		}
#else
		slv::console_log(slv::LOG_WARNING, M_CLASS_NAME, "SLV's debug console is not supported on this platform");
#endif
#endif
	}

	// private
	void WindowHandler::configure_flags(bool window_has_vsync, bool is_window_resizable, bool window_starts_fullscreen, bool is_window_borderless, bool is_window_transparent)
	{
#ifdef _WIN32
		if (is_window_transparent)
		{
			is_window_borderless = true;
			is_window_resizable = false;
			window_starts_fullscreen = false; // transparent windows cannot be fullscreen
		}
#else
		is_window_transparent = false;
		slv::console_log(slv::LOG_WARNING, M_CLASS_NAME, "SLV's transparent window feature is not supported on this platform");
#endif

		int flags = 0;

		if (window_has_vsync)
		{
			flags |= FLAG_VSYNC_HINT;
		}

		if (is_window_borderless)
		{
			is_window_resizable = false;
			flags |= FLAG_WINDOW_UNDECORATED;
		}

		if (is_window_resizable)
		{
			flags |= FLAG_WINDOW_RESIZABLE;
		}

		if (window_starts_fullscreen)
		{
			flags |= FLAG_FULLSCREEN_MODE;
		}

		SetConfigFlags(flags);
	}

	// private
	void WindowHandler::create_buffers()
	{
		if (m_view.id != 0u)
		{
			slv::raylib::unload_render_texture(m_view);
		}

		m_view = slv::raylib::load_render_texture(m_current_window_size.width, m_current_window_size.height);
#ifdef _WIN32
		m_render_buffers = slv::win32::create_render_buffers(m_current_window_size.width, m_current_window_size.height);
#endif
	}

	// private
	void WindowHandler::reset_minimum_window_size()
	{
		m_minimum_window_size.width = std::min(m_default_window_size.width, M_LOW_WINDOW_SIZE.width);
		m_minimum_window_size.height = std::min(m_default_window_size.height, M_LOW_WINDOW_SIZE.height);
		SetWindowMinSize(m_minimum_window_size.width, m_minimum_window_size.height);
	}
}