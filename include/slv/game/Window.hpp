#pragma once

#include <slv/core/types/window_settings.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <string>
#include <vector>

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
        void toggle_fullscreen(bool val);
        bool is_fullscreen() const;
        bool is_transparent() const;
        bool is_resizable() const;
        bool is_borderless() const;
        bool has_vsync() const;
#ifdef _WIN32
        void open_console() const;
        void close_console() const;
#endif

        inline int running_fps() const
        {
            return m_running_fps;
        }

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

        std::string title() const
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
        void update(float dt);
        void start_draw() const;
        void end_draw() const;
        void configure_settings(int settings);
        void reset_minimum_size();

        static constexpr inline const char* M_NAME = "Window";
        static constexpr inline int M_LOWEST_SIZE_PX = 400;
        static constexpr inline slv::size<int> M_LOW_SIZE{ 800, 600 };
        slv::vec2<int> m_pos{};
        slv::size<int> m_default_size{};
        slv::size<int> m_current_size{};
        slv::size<int> m_minimum_size{};
        slv::size<int> m_last_current_size{};
        slv::size<int> m_unmaximized_size{};
        std::string m_title;
        int m_target_fps = 0;
        int m_running_fps = 0;
        int m_frame_count = 0;
        float m_elapsed = 0.f;
        bool m_is_initialized = false;
	};
}