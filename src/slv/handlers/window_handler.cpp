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
							 bool has_vsync, bool is_resizable, bool start_fullscreen, bool is_borderless, bool is_transparent)
	{
		if (m_is_init)
		{
			return true;
		}

		setup_console(window_title);
		configure_flags(has_vsync, is_resizable, start_fullscreen, is_borderless, is_transparent);

		slv::size<unsigned int> normalized_win_size = window_size;
		// LOWEST MINIMUM SIZE
		normalized_win_size.width = std::max(normalized_win_size.width, 100U);
		normalized_win_size.height = std::max(normalized_win_size.height, 100U);
		InitWindow(normalized_win_size.width, normalized_win_size.height, window_title.c_str());

		bool window_failed = !IsWindowReady() || !GetWindowHandle();
		if (window_failed)
		{
			slv::console_log(slv::LOG_ERROR, M_CLASS_NAME, "Failed to initialize window");
			return false;
		}
	
		m_default_win_size = normalized_win_size;
		m_win_title = window_title;
		m_target_fps = fps;
		m_is_fullscreen = start_fullscreen;
		m_is_transparent = is_transparent;

		// setup transparency
		if (m_is_transparent)
		{
			create_buffers();
			slv::win32::init_layered_window(GetWindowHandle());
		}

		unsigned int min_width = std::min(static_cast<unsigned int>(m_default_win_size.width), 800U);
		unsigned int min_height = std::min(static_cast<unsigned int>(m_default_win_size.height), 600U);
		m_min_win_size = slv::size<unsigned int>(min_width, min_height);
		SetWindowMinSize(m_min_win_size.width, m_min_win_size.height);
		SetWindowMaxSize(get_monitor_size().width, get_monitor_size().height);
		SetTargetFPS(m_target_fps);
		SetExitKey(KEY_NULL); // disable closing the window when ESC is pressed

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

		slv::raylib::unload_render_texture(m_target);

		if (m_is_transparent)
		{
#ifdef _WIN32
			slv::win32::cleanup_layered_window();
#endif
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

		m_current_win_size = slv::size<unsigned int>(static_cast<unsigned int>(GetRenderWidth()), static_cast<unsigned int>(GetRenderHeight()));

		if (m_is_transparent && m_current_win_size != m_last_current_win_size)
		{
			create_buffers();
			m_last_current_win_size = m_current_win_size;
		}

		m_win_pos = slv::vec_2<int>(static_cast<int>(GetWindowPosition().x), static_cast<int>(GetWindowPosition().y));

#ifdef _WIN32
		// windows automatically fullscreens the window when the size is the same as the monitor's and if the window position is (0, 0)
		// this disables that in case if the window is not supposed to be fullscreen
		if (!m_is_fullscreen && m_win_pos == slv::vec_2<int>(0, 0) && m_current_win_size == get_monitor_size())
		{
			set_pos(slv::vec_2<int>(0, -1));
		}
#endif

		// update fullscreen
		// transparent windows cannot be fullscreen
		if (!m_is_transparent && IsKeyPressed(KEY_F11))
		{
			if (!IsWindowFullscreen()) // going fullscreen
			{
				m_unmaximized_win_size = m_current_win_size;
				SetWindowSize(get_monitor_size().width, get_monitor_size().height);
				m_is_fullscreen = true;
			}
			else // leaving fullscreen
			{
				SetWindowSize(m_unmaximized_win_size.width, m_unmaximized_win_size.height);
				m_is_fullscreen = false;
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
		BeginScissorMode(0, 0, m_current_win_size.width, m_current_win_size.height);

		if (m_is_transparent)
		{
			slv::raylib::begin_texture_mode(m_target);
			ClearBackground(BLANK);
			return;
		}

		ClearBackground(BLACK);
	}

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
		DrawText(fmt::format("FPS: {}", get_fps()).c_str(), text_border_padding, text_border_padding, text_size, WHITE);

		float memory_usage = 0.f;
#ifdef _WIN32
		memory_usage = slv::win32::get_memory_mb();
#endif
		if (memory_usage != 0.f)
		{
			DrawText(fmt::format("MEM: {:.2f}MB", memory_usage).c_str(), text_border_padding, text_border_padding + (text_padding * 2), text_size, WHITE);
		}
#endif

		if (m_is_transparent)
		{
			EndTextureMode();
			Image img = LoadImageFromTexture(Texture(m_target.tex.id, m_target.tex.width, m_target.tex.height, m_target.tex.mipmaps, m_target.tex.format));
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

	bool WindowHandler::is_open() const
	{
		if (m_is_init)
		{
			return !WindowShouldClose();
		}

		return false;
	}

	void WindowHandler::set_size(const slv::size<unsigned int>& size)
	{
		set_width(size.width);
		set_height(size.height);
	}

	void WindowHandler::set_width(unsigned int width)
	{
		if (!m_is_init)
		{
			return;
		}

		m_default_win_size.width = width;
		if (m_min_win_size.width > m_default_win_size.width)
		{
			m_default_win_size.width = m_min_win_size.width;
		}

		SetWindowSize(m_default_win_size.width, m_default_win_size.height);
	}

	void WindowHandler::set_height(unsigned int height)
	{
		if (!m_is_init)
		{
			return;
		}

		m_default_win_size.height = height;
		if (m_min_win_size.height > m_default_win_size.height)
		{
			m_default_win_size.height = m_min_win_size.height;
		}

		SetWindowSize(m_default_win_size.width, m_default_win_size.height);
	}

	void WindowHandler::set_title(std::string_view title)
	{
		if (m_is_init)
		{
			m_win_title = title;
			SetWindowTitle(m_win_title.c_str());
			
#ifdef _WIN32
			if (slv::win32::is_console_open())
			{
				slv::win32::rename_console(m_win_title);
			}
#endif
		}
	}

	void WindowHandler::set_target_fps(unsigned int fps)
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
			return
			{
				static_cast<unsigned int>(GetMonitorWidth(monitor)),
				static_cast<unsigned int>(GetMonitorHeight(monitor)),
			};
		}

		return {};
	}

	float WindowHandler::get_delta_time() const
	{
		if (m_is_init)
		{
			return GetFrameTime();
		}

		return 0.f;
	}

	slv::vec_2<float> WindowHandler::get_mouse_pos() const
	{
		if (m_is_init)
		{
			Vector2 pos = GetMousePosition();
			float ui_scale = std::max(get_ui_scale(), 1e-6f);
			return { pos.x / ui_scale, pos.y / ui_scale };
		}

		return {};
	}

	slv::vec_2<float> WindowHandler::get_mouse_delta() const
	{
		if (m_is_init)
		{
			Vector2 dt = GetMouseDelta();
			return { dt.x, dt.y };
		}

		return {};
	}

	float WindowHandler::get_ui_scale() const
	{
		float w = m_default_win_size.width > 0 ? static_cast<float>(m_current_win_size.width) / m_default_win_size.width : 1.f;
		float h = m_default_win_size.height > 0 ? static_cast<float>(m_current_win_size.height) / m_default_win_size.height : 1.f;
		return std::min(w, h);
	}

	int WindowHandler::get_fps() const
	{
		float dt = get_delta_time();
		if (dt != 0.f)
		{
			return static_cast<int>(round(1.f / dt));
		}

		return 0;
	}

	void WindowHandler::set_pos(const slv::vec_2<int>& pos)
	{
		if (m_is_init && !IsWindowFullscreen())
		{
			m_win_pos = pos;
			SetWindowPosition(pos.x, pos.y);
		}
	}

	void WindowHandler::set_pos_x(int x)
	{
		set_pos(slv::vec_2<int>(x, m_win_pos.y));
	}

	void WindowHandler::set_pos_y(int y)
	{
		set_pos(slv::vec_2<int>(m_win_pos.x, y));
	}

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

	void WindowHandler::configure_flags(bool has_vsync, bool is_resizable, bool start_fullscreen, bool is_borderless, bool is_transparent)
	{
#ifdef _WIN32
		if (is_transparent)
		{
			is_borderless = true;
			is_resizable = false;
			start_fullscreen = false; // transparent windows cannot be fullscreen
		}
#else
		is_transparent = false;
		slv::console_log(slv::LOG_WARNING, M_CLASS_NAME, "SLV's transparent window feature is not supported on this platform");
#endif

		int flags = 0;

		if (has_vsync)
		{
			flags |= FLAG_VSYNC_HINT;
		}

		if (is_borderless)
		{
			is_resizable = false;
			flags |= FLAG_WINDOW_UNDECORATED;
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
	}

	void WindowHandler::create_buffers()
	{
		if (m_target.id != 0u)
		{
			slv::raylib::unload_render_texture(m_target);
		}

		m_target = slv::raylib::load_render_texture(m_current_win_size.width, m_current_win_size.height);
#ifdef _WIN32
		m_render_buffers = slv::win32::create_render_buffers(m_current_win_size.width, m_current_win_size.height);
#endif
	}
}