#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/main/Context.hh>
#include <cruxus/engine/math/vec2.hh>
#include <cruxus/engine/math/size.hh>
#include <string>
#include <string_view>

namespace crx {
// TODO: make this a struct instead

//enum window_settings : int {
//    none = 0,
//    vsync = 1 << 0,
//    unresizable = 1 << 1,
//    start_fullscreen = 1 << 2,
//    borderless = 1 << 3,
//    transparent = 1 << 4
//};

class Game; // forward declare
class CRX_DLL Window final {
    friend class Game;
private:
    Window();
    ~Window();

public:
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator =(Window const&) = delete;
    Window& operator =(Window&&) = delete;

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

    struct CRX_DLL config {
        config();
        explicit config(
            bool vsync_val,
            bool unresizable_val,
            bool start_fullscreen_val,
            bool borderless_val,
            bool transparent_val
        );

        bool vsync;
        bool unresizable;
        bool start_fullscreen;
        bool borderless;
        bool transparent;
    };

private:
    bool init(
        std::string_view title,
        size<int> const& size,
        int fps,
        config& cnfg,
        Context const& ctx
    );
    void shutdown();
    void update(Context const& ctx, float dt);
    void start_draw() const;
    void end_draw();
    void configure_configurations(config& cnfg);

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
#if (defined(CRUXUS_DEBUG) || defined(CRUXUS_RELWITHDEBINFO))
    std::string debug_text_;
    float memory_usage_;
    float max_memory_usage_;
#endif
    bool invoked_fullscreen_;
    bool was_resized_;
    bool is_initialized_;
};
}