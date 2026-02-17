#pragma once

#include <slv/core/types/window_settings.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <string>
#include <vector>

#ifdef _WIN32
#include <platform/windows/layered_window.hpp>
#endif

namespace slv
{
    class Game; // forward declare

	class Window
	{
        friend class slv::Game;

    private:
        Window() = default;
        ~Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

    public:
        bool is_open() const;
        slv::vec2<float> screen_center() const;
        slv::size<float> screen_size() const;
        slv::size<int> monitor_size() const;
        slv::vec2<float> mouse_pos() const;
        slv::vec2<float> mouse_delta() const;
        int running_fps() const;
        float delta_time() const;
        float ui_scale() const;
        void set_size(const slv::size<int>& size, bool set_as_default = true);
        void set_width(int width, bool set_as_default = true);
        void set_height(int height, bool set_as_default = true);
        void set_fps(int fps);
        void set_title(const std::string& title);
        void set_pos(const slv::vec2<int>& pos);
        void set_posx(int x);
        void set_posy(int y);
        bool is_fullscreen() const;

        inline slv::size<int> size() const
        {
            return m_current_size;
        }

        inline slv::size<int> minimum_size() const
        {
            return m_minimum_size;
        }

        inline slv::size<int> default_size() const
        {
            return m_default_size;
        }

        inline std::string title() const
        {
            return m_title;
        }

        inline int target_fps() const
        {
            return m_target_fps;
        }

        inline slv::vec2<int> pos() const
        {
            return m_pos;
        }

    private:
        bool init(const std::string& title, const slv::size<int>& size, int fps, int settings);
        void uninit();
        void update();
        void start_draw() const;
        void end_draw();
        void setup_console(const std::string& window_title);
        void configure_settings(int settings);
        void create_buffers();
        void reset_minimum_size();

        static constexpr inline const char* M_NAME = "Window";
        static constexpr inline int M_LOWEST_SIZE_PX = 100;
        static constexpr inline slv::size<int> M_LOW_SIZE{ 800, 600 };
        bool m_is_init = false;
        slv::vec2<int> m_pos{};
        slv::size<int> m_default_size{};
        slv::size<int> m_current_size{};
        slv::size<int> m_minimum_size{};
        slv::size<int> m_last_current_size{};
        slv::size<int> m_unmaximized_size{};
        slv::render_texture m_view{};
#ifdef _WIN32
        slv::win32::render_buffers m_render_buffers{};
#endif
        std::string m_title;
        int m_target_fps = 0;
        bool m_is_transparent = false;
        bool m_is_fullscreen = false;
	};
}