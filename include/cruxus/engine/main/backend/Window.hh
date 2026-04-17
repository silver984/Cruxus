#pragma once
#include <slv/config.hh>
#include <slv/types/context.hpp>
#include <slv/engine/math/vec2.hh>
#include <slv/engine/math/size.hh>
#include <string>
#include <string_view>

namespace crx {

enum window_settings : int {
    none = 0,
    vsync = 1 << 0,
    unresizable = 1 << 1,
    start_fullscreen = 1 << 2,
    borderless = 1 << 3,
    transparent = 1 << 4
};

class Game; // forward declare
class SLV_DLL Window final {
    friend class Game;

private:
    Window();
    ~Window();

public:
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] std::string_view title() const;
    [[nodiscard]] size<float> screen_size() const;
    [[nodiscard]] size<int> monitor_size() const;
    [[nodiscard]] size<int> draw_size() const;
    [[nodiscard]] size<int> default_screen_size() const;
    [[nodiscard]] vec2<float> screen_center() const;
    [[nodiscard]] vec2<float> mouse_pos() const;
    [[nodiscard]] vec2<float> mouse_delta() const;
    [[nodiscard]] vec2<int> pos() const;
    [[nodiscard]] float delta_time() const;
    [[nodiscard]] float ui_scale() const;
    [[nodiscard]] bool is_open() const;
    [[nodiscard]] bool is_fullscreen() const;
    [[nodiscard]] bool is_transparent() const;
    [[nodiscard]] bool is_resizable() const;
    [[nodiscard]] bool is_borderless() const;
    [[nodiscard]] bool is_minimized() const;
    [[nodiscard]] bool has_vsync() const;
    [[nodiscard]] bool was_resized() const;
    [[nodiscard]] int running_fps() const;
    [[nodiscard]] int target_fps() const;
    void set_size(size<int> const& size, bool set_as_default = true);
    void set_width(int width, bool set_as_default = true);
    void set_height(int height, bool set_as_default = true);
    void set_fps(int fps);
    void set_title(std::string_view title);
    void set_pos(vec2<int> const& pos);
    void set_pos_x(int x);
    void set_pos_y(int y);
    void toggle_fullscreen(bool val);

private:
    bool init(
        std::string_view title,
        size<int> const& size,
        int fps,
        window_settings settings,
        context const& ctx
    );
    void uninit();
    void update(context const& ctx, float dt);
    void start_draw() const;
    void end_draw() const;
    void configure_settings(window_settings settings);

    std::string title_;
    vec2<int> pos_;
    size<int> default_screen_size_;
    size<int> draw_size_;
    size<int> last_draw_size_;
    size<int> unmaximized_size_;
    int target_fps_;
    int running_fps_;
    int frame_count_;
    float elapsed_second_;
#ifdef CRX_DEBUG
    float memory_usage_;
    float max_memory_usage_;
#endif
    bool invoked_fullscreen_;
    bool was_resized_;
    bool is_initialized_;
};

}