#pragma once

#include <slv/core/types.hpp>
#ifdef _WIN32
#include <platform/windows/layered_window.hpp>
#endif
#include <string>
#include <vector>

namespace slv
{
	class WindowHandler
	{
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
                  bool has_vsync = true, bool is_resizable = true, bool start_fullscreen = false, bool is_borderless = false, bool is_transparent = false);
        void uninit();
        void update();
        void start_draw() const;
        void end_draw();
        bool is_open() const;
        slv::size<unsigned int> get_monitor_size() const;
        slv::vec_2<float> get_mouse_pos() const;
        slv::vec_2<float> get_mouse_delta() const;
        int get_fps() const;
        float get_delta_time() const;
        float get_ui_scale() const;
        void set_size(const slv::size<unsigned int>& size);
        void set_width(unsigned int width);
        void set_height(unsigned int height);
        void set_target_fps(unsigned int fps);
        void set_title(std::string_view title);
        void set_pos(const slv::vec_2<int>& pos);
        void set_pos_x(int x);
        void set_pos_y(int y);

        inline const slv::size<unsigned int>& get_size() const
        {
            return m_current_win_size;
        }

        inline const slv::size<unsigned int>& get_min_size() const
        {
            return m_min_win_size;
        }
        
        inline slv::vec_2<float> get_center() const
        {
            float ui_scale = get_ui_scale();
            return
            {
                (m_current_win_size.width / 2.0F) / ui_scale,
                (m_current_win_size.height / 2.0F) / ui_scale
            };
        }

        inline const slv::size<unsigned int>& get_def_size() const
        {
            return m_default_win_size;
        }

        inline const std::string& get_title() const
        {
            return m_win_title;
        }

        inline unsigned int get_target_fps() const
        {
            return m_target_fps;
        }

        inline const slv::vec_2<int>& get_pos() const
        {
            return m_win_pos;
        }

    private:
        void setup_console(const std::string& window_title);
        void configure_flags(bool has_vsync, bool is_resizable, bool start_fullscreen, bool is_borderless, bool is_transparent);
        void create_buffers();

        static constexpr inline const char* M_CLASS_NAME = "WindowHandler";
        bool m_is_init = false;
        slv::vec_2<int> m_win_pos{};
        slv::size<unsigned int> m_default_win_size{};
        slv::size<unsigned int> m_current_win_size{};
        slv::size<unsigned int> m_min_win_size{};
        slv::size<unsigned int> m_last_current_win_size{};
        slv::size<unsigned int> m_unmaximized_win_size{};
        slv::render_texture m_target{};
        slv::render_buffers m_render_buffers{};
        std::string m_win_title;
        unsigned int m_target_fps = 0U;
        bool m_is_transparent = false;
        bool m_is_fullscreen = false;
	};
}