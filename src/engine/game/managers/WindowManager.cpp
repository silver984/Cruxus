#include <slv/engine/Game/managers/Window.hpp>
#include <slv/engine/Game/managers/AudioManager.hpp>
#include <fmt/format.h>
#include <slv/engine/log.hpp>
#include "rl.hpp"
#ifdef _WIN32
#include "platforms/windows.hpp"
#endif
#include <raylib.h>
#include <algorithm>
#include <cstddef>
#include <cmath>

// this whole namespace is here just so raylib has
// its own distinct messenger in the logs instead of
// "slv::Window::init"
namespace raylib {

void log(int level, char const* msg, va_list args) {
	if (level == LOG_TRACE || level == LOG_INFO) {
		return;
	}

	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	switch (level) {
	case LOG_WARNING:
		slv::log::warning(buffer);
		break;

	case LOG_ERROR:
		slv::log::error(buffer);
		break;

	default: break;
	}
}

}

namespace crx {

// private
Window::Window() :
	target_fps_(0),
	running_fps_(0),
	frame_count_(0),
	elapsed_second_(0.f),
	memory_usage_(0.f),
	max_memory_usage_(0.f),
	invoked_fullscreen_(false),
	was_resized_(false),
	is_initialized_(false)
{}

// private
Window::~Window() = default;

// private
bool Window::init(
	std::string_view title,
	size<int> const& size,
	int fps,
	window_settings settings,
	context const& ctx
) {
	if (is_initialized_) {
		return true;
	}

#if (defined(CRX_DEBUG) || defined(CRX_RELWITHDEBINFO))
	if (!win32::enable_console_colors()) {
		log::warning("Couldn't enable console colors");
	}
#endif

	SetTraceLogCallback(::raylib::log);
	configure_settings(settings);

	title_ = std::string(title);
	default_screen_size_.width = std::max(1, size.width);
	default_screen_size_.height = std::max(1, size.height);
	target_fps_ = std::max(1, fps);

	if (!rl::init_window(default_screen_size_, target_fps_, title_.c_str())) {
		return false;
	}

	is_initialized_ = true;
	log::info("Window initialized");
	update(ctx, 0.f);

	return true;
}

// private
void Window::uninit() {
	if (!is_initialized_) {
		return;
	}

	is_initialized_ = false;
	log::info("Destroying Window...");
	CloseWindow();
}

// private
void Window::update(context const& ctx, float dt) {
	if (!is_initialized_) {
		return;
	}

	draw_size_ = size<int>(GetRenderWidth(), GetRenderHeight());

	if (last_draw_size_ != draw_size_) {
		was_resized_ = true;
		last_draw_size_ = draw_size_;
	} else {
		was_resized_ = false;
	}

	pos_ = vec2<int>(
		static_cast<int>(GetWindowPosition().x),
		static_cast<int>(GetWindowPosition().y)
	);

#ifdef _WIN32
	// it seems that on windows,
	// a Window automatically fullscreens the Window when the size is the same as the monitor's and if the Window position is (0, 0)
	// this disables that in case if the Window is not supposed to be fullscreen
	if (
		!is_fullscreen() &&
		pos_ == vec2<int>(0, 0) &&
		draw_size_ == monitor_size()
	) {
		set_pos(vec2<int>(0, -1));
	}
#endif

	if (IsKeyPressed(KEY_F11)) {
		invoked_fullscreen_ = !invoked_fullscreen_;
		toggle_fullscreen(invoked_fullscreen_);
	}

	elapsed_second_ += dt;
	frame_count_++;

	while (elapsed_second_ >= 1.f) {
		running_fps_ = frame_count_;
		frame_count_ = 0;
		elapsed_second_ -= 1.f;
	}

#if (defined(CRX_DEBUG) || defined(CRX_RELWITHDEBINFO)) && _WIN32
	memory_usage_ = win32::proc_memory_mb();
	max_memory_usage_ = std::max(memory_usage_, max_memory_usage_);
#endif
}

// private
void Window::start_draw() const {
	if (!is_initialized_) {
		return;
	}

	BeginDrawing();
	BeginScissorMode(0, 0, draw_size_.width, draw_size_.height);

	if (is_transparent()) {
		ClearBackground(BLANK);
		return;
	}

	ClearBackground(BLACK);
}

// private
void Window::end_draw() const {
	if (!is_initialized_) {
		return;
	}

#if defined(CRX_DEBUG) || defined(CRX_RELWITHDEBINFO)
	std::string debug_text = fmt::format("FPS: {}", running_fps());

	if (memory_usage_ != 0.f) {
		debug_text += fmt::format(
			"\nMEM: {:.2f}MB / {:.2f}MB",
			memory_usage_,
			max_memory_usage_
		);
	}

	int text_size = 10;
	int text_border_padding = 5;
	DrawText(
		debug_text.c_str(),
		text_border_padding,
		text_border_padding,
		text_size,
		WHITE
	);
#endif

	EndScissorMode();
	EndDrawing();
}

bool Window::is_open() const {
	if (is_initialized_) {
		return !WindowShouldClose();
	}

	return false;
}

vec2<float> Window::screen_center() const {
	float ui_scale_val = ui_scale();
	return vec2<float>(
		(draw_size_.width / 2.f) / ui_scale_val,
		(draw_size_.height / 2.f) / ui_scale_val
	);
}

size<float> Window::screen_size() const {
	float ui_scale_val = ui_scale();
	return size<float>(
		draw_size_.width / ui_scale_val,
		draw_size_.height / ui_scale_val
	);
}

// TODO: fix Window sizing

void Window::set_size(size<int> const& size, bool set_as_default) {
	set_width(size.width, set_as_default);
	set_height(size.height, set_as_default);
}

void Window::set_width(int width, bool set_as_default) {
	if (!is_initialized_) {
		return;
	}

	if (set_as_default) {
		default_screen_size_.width = std::max(1, width);
	}

	SetWindowSize(set_as_default ? default_screen_size_.width : std::max(1, width), default_screen_size_.height);
	was_resized_ = true;
}

void Window::set_height(int height, bool set_as_default) {
	if (!is_initialized_) {
		return;
	}

	if (set_as_default) {
		default_screen_size_.height = std::max(1, height);
	}

	SetWindowSize(default_screen_size_.width, set_as_default ? default_screen_size_.height : std::max(1, height));
	was_resized_ = true;
}

void Window::set_title(std::string_view title) {
	if (is_initialized_) {
		title_ = title;
		SetWindowTitle(title_.c_str());
	}
}

void Window::set_fps(int fps) {
	if (is_initialized_) {
		target_fps_ = std::max(1, fps);
		SetTargetFPS(target_fps_);
	}
}

size<int> Window::monitor_size() const {
	if (is_initialized_) {
		int monitor = GetCurrentMonitor();
		return size<int>(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	}

	return {};
}

float Window::delta_time() const {
	if (!is_initialized_) {
		return 0.f;
	}

	float dt = GetFrameTime();
	float dt_spike = 0.25f;
	if (dt > dt_spike) {
		return 0.f;
	}

	return dt;
}

vec2<float> Window::mouse_pos() const {
	if (is_initialized_) {
		Vector2 pos = GetMousePosition();
		float ui_scale_val = ui_scale();
		return ui_scale_val != 0.f
			? vec2<float>(pos.x / ui_scale_val, pos.y / ui_scale_val)
			: vec2<float>(pos.x, pos.y);
	}

	return {};
}

vec2<float> Window::mouse_delta() const {
	if (is_initialized_) {
		Vector2 dt = GetMouseDelta();
		return vec2<float>(dt.x, dt.y);
	}

	return {};
}

float Window::ui_scale() const {
	float w = default_screen_size_.width > 0
		? static_cast<float>(draw_size_.width) / default_screen_size_.width
		: 1.f;

	float h = default_screen_size_.height > 0
		? static_cast<float>(draw_size_.height) / default_screen_size_.height
		: 1.f;
	
	return std::min(w, h);
}

void Window::set_pos(const vec2<int>& pos) {
	if (is_initialized_ && !IsWindowFullscreen()) {
		pos_ = pos;
		SetWindowPosition(pos_.x, pos_.y);
	}
}

void Window::set_pos_x(int x) {
	set_pos(vec2<int>(x, pos_.y));
}

void Window::set_pos_y(int y) {
	set_pos(vec2<int>(pos_.x, y));
}

void Window::toggle_fullscreen(bool val) {
	if (is_transparent()) {
		return;
	}

	if (val && !IsWindowFullscreen()) {
		unmaximized_size_ = draw_size_;
		auto monitor_size_val = monitor_size();
		SetWindowSize(monitor_size_val.width, monitor_size_val.height);
		ToggleFullscreen();
		return;
	}

	SetWindowSize(unmaximized_size_.width, unmaximized_size_.height);
	ToggleFullscreen();
}

bool Window::is_fullscreen() const {
	if (is_initialized_) {
		return IsWindowFullscreen();
	}

	return false;
}

bool Window::is_transparent() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_TRANSPARENT);
	}

	return false;
}

bool Window::is_resizable() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_RESIZABLE);
	}

	return false;
}

bool Window::is_borderless() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_UNDECORATED);
	}

	return false;
}

bool Window::is_minimized() const {
	if (is_initialized_) {
		return IsWindowMinimized();
	}

	return false;
}

bool Window::has_vsync() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_VSYNC_HINT);
	}

	return false;
}

bool Window::was_resized() const {
	return was_resized_;
}

int Window::running_fps() const {
	return running_fps_;
}

size<int> Window::draw_size() const {
	return draw_size_;
}

size<int> Window::default_screen_size() const {
	return default_screen_size_;
}

std::string_view Window::title() const {
	return title_;
}

int Window::target_fps() const {
	return target_fps_;
}

vec2<int> Window::pos() const {
	return pos_;
}

// private
void Window::configure_settings(window_settings settings) {
	using enum window_settings;
	bool is_vsync = (settings & VSYNC) != NONE;
	bool is_unresizable = (settings & UNRESIZABLE) != NONE;
	bool is_start_fullscreen = (settings & START_FULLSCREEN) != NONE; // TODO: fix startup with this setting on
	bool is_borderless = (settings & BORDERLESS) != NONE;
	bool is_transparent = (settings & TRANSPARENT) != NONE;
	int flags = 0;

	if (is_transparent) {
		flags |= FLAG_WINDOW_TRANSPARENT;
		is_start_fullscreen = false;
		is_borderless = true;
	}

	if (is_vsync) {
		flags |= FLAG_VSYNC_HINT;
	}

	if (is_borderless) {
		is_unresizable = true;
		flags |= FLAG_WINDOW_UNDECORATED;
	}

	bool resizable = !is_unresizable; // just for clarity
	if (resizable) {
		flags |= FLAG_WINDOW_RESIZABLE;
	}

	if (is_start_fullscreen) {
		flags |= FLAG_FULLSCREEN_MODE;
	}

	SetConfigFlags(flags);
}

} // namespace crx