#pragma once

#include <slv/core/types.hpp>
#ifdef _WIN32
#include <platform/windows/layered_window.hpp>
#endif
#include <string>
#include <vector>

namespace slv
{
    class Game; // forward declare

	class WindowHandler
	{
        friend class slv::Game;

    private:
        WindowHandler() = default;
        ~WindowHandler();

    public:
        static inline WindowHandler& get()
        {
            static WindowHandler instance;
            return instance;
        }

        WindowHandler(const WindowHandler&) = delete;
        WindowHandler(WindowHandler&&) = delete;
        WindowHandler& operator = (const WindowHandler&) = delete;
        WindowHandler& operator = (WindowHandler&&) = delete;

        bool init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps = 60U,
                  bool window_has_vsync = true, bool is_window_resizable = true, bool window_starts_fullscreen = false, bool is_window_borderless = false, bool is_window_transparent = false);
        void uninit();
        bool is_window_open() const;
        slv::size<unsigned int> get_monitor_size() const;
        slv::vec_2<float> get_mouse_pos() const;
        slv::vec_2<float> get_mouse_delta() const;
        int get_running_fps() const;
        float get_delta_time() const;
        float get_ui_scale() const;
        void set_window_size(const slv::size<unsigned int>& size, bool set_as_default = true);
        void set_window_width(unsigned int width, bool set_as_default = true);
        void set_window_height(unsigned int height, bool set_as_default = true);
        void set_fps(unsigned int fps);
        void set_window_title(const std::string& title);
        void set_window_pos(const slv::vec_2<int>& pos);
        void set_window_pos_x(int x);
        void set_window_pos_y(int y);
        bool is_window_fullscreen() const;

        inline slv::size<unsigned int> get_window_size() const
        {
            return m_current_window_size;
        }

        inline slv::size<unsigned int> get_window_minimum_size() const
        {
            return m_minimum_window_size;
        }

        inline slv::vec_2<float> get_screen_center() const
        {
            float ui_scale = get_ui_scale();
            return slv::vec_2<float>((m_current_window_size.width / 2.0F) / ui_scale,
                                     (m_current_window_size.height / 2.0F) / ui_scale);
        }

        inline slv::size<unsigned int> get_window_default_size() const
        {
            return m_default_window_size;
        }

        inline std::string get_window_title() const
        {
            return m_window_title;
        }

        inline unsigned int get_target_fps() const
        {
            return m_target_fps;
        }

        inline slv::vec_2<int> get_window_pos() const
        {
            return m_window_pos;
        }

    private:
        void update();
        void start_draw() const;
        void end_draw();
        void setup_console(const std::string& window_title);
        void configure_flags(bool window_has_vsync, bool is_window_resizable, bool window_starts_fullscreen, bool is_window_borderless, bool is_window_transparent);
        void create_buffers();
        void reset_minimum_window_size();

        static constexpr inline const char* M_CLASS_NAME = "WindowHandler";
        static constexpr inline unsigned int M_LOWEST_WINDOW_SIZE_PX = 100U;
        static constexpr inline slv::size<unsigned int> M_LOW_WINDOW_SIZE{ 800U, 600U };
        bool m_is_init = false;
        slv::vec_2<int> m_window_pos{};
        slv::size<unsigned int> m_default_window_size{};
        slv::size<unsigned int> m_current_window_size{};
        slv::size<unsigned int> m_minimum_window_size{};
        slv::size<unsigned int> m_last_current_window_size{};
        slv::size<unsigned int> m_unmaximized_window_size{};
        slv::render_texture m_view{};
        slv::render_buffers m_render_buffers{};
        std::string m_window_title;
        unsigned int m_target_fps = 0U;
        bool m_is_window_transparent = false;
        bool m_is_window_fullscreen = false;
	};
}