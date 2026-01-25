#pragma once

#include <slv/types.hpp>
#include <string>
#include <string_view>

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

        bool init(std::string_view window_title, const slv::size_uint& window_size, unsigned int fps, bool has_vsync = true, bool is_resizable = true, bool start_fullscreen = false);
        void uninit();
        void update();
        void start_draw() const;
        void end_draw() const;
        bool should_close() const;
        
        inline const slv::size_uint& get_window_size() const
        {
            return m_current_win_size;
        }

        void set_window_size(const slv::size_uint& size);

        inline const std::string& get_window_title() const
        {
            return m_win_title;
        }

        void set_window_title(std::string_view title);

        inline unsigned int get_fps() const
        {
            return m_fps;
        }

        void set_fps(unsigned int fps);

        inline const bool is_active() const
        {
            return m_is_init;
        }

        const slv::size_uint get_monitor_size() const;
        const float get_delta_time() const;
        const slv::vec_2 get_mouse_pos() const;
        const slv::vec_2 get_mouse_delta() const;
        float get_ui_scale() const;

    private:
        static constexpr const char* m_class_name = "WindowHandler";
        bool m_is_init = false;
        slv::size_uint m_original_win_size{};
        slv::size_uint m_current_win_size{};
        slv::size_uint m_last_win_size{};
        std::string m_win_title;
        unsigned int m_fps = 0u;
	};
}