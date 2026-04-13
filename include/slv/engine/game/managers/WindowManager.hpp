#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/primitives.hpp>
#include <slv/types/context.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace slv {

enum window_settings : int {
    NONE = 0,
    VSYNC = 1 << 0,
    UNRESIZABLE = 1 << 1,
    START_FULLSCREEN = 1 << 2,
    BORDERLESS = 1 << 3,
    TRANSPARENT = 1 << 4
};

class App; // forward declare
class SLV_DLL WindowManager final {
    friend class App;

private:
    WindowManager();
    ~WindowManager();

public:
    WindowManager(WindowManager const&) = delete;
    WindowManager(WindowManager&&) = delete;
    WindowManager& operator=(WindowManager const&) = delete;
    WindowManager& operator=(WindowManager&&) = delete;

    [[nodiscard]] vec2<float> screen_center() const;
    [[nodiscard]] size<float> screen_size() const;
    [[nodiscard]] size<int> monitor_size() const;
    [[nodiscard]] vec2<float> mouse_pos() const;
    [[nodiscard]] vec2<float> mouse_delta() const;
    [[nodiscard]] float delta_time() const;
    [[nodiscard]]  float ui_scale() const;
    void set_size(size<int> const& size, bool set_as_default = true);
    void set_width(int width, bool set_as_default = true);
    void set_height(int height, bool set_as_default = true);
    void set_fps(int fps);
    void set_title(std::string_view title);
    void set_pos(vec2<int> const& pos);
    void set_pos_x(int x);
    void set_pos_y(int y);
    void toggle_fullscreen(bool val);
    [[nodiscard]] bool is_open() const;
    [[nodiscard]] bool is_fullscreen() const;
    [[nodiscard]] bool is_transparent() const;
    [[nodiscard]] bool is_resizable() const;
    [[nodiscard]] bool is_borderless() const;
    [[nodiscard]] bool has_vsync() const;
    [[nodiscard]] bool was_resized() const;
    [[nodiscard]] int running_fps() const;
    [[nodiscard]] size<int> draw_size() const;
    [[nodiscard]] size<int> default_screen_size() const;
    [[nodiscard]] std::string_view title() const;
    [[nodiscard]] int target_fps() const;
    [[nodiscard]] vec2<int> pos() const;

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

    vec2<int> pos_;
    size<int> default_screen_size_;
    size<int> draw_size_;
    size<int> last_draw_size_;
    size<int> unmaximized_size_;
    std::string title_;
    int target_fps_;
    int running_fps_;
    int frame_count_;
    float elapsed_second_;
#ifdef SLV_DEBUG
    float memory_usage_;
    float max_memory_usage_;
#endif
    bool invoked_fullscreen_;
    bool was_resized_;
    bool is_initialized_;
};

}