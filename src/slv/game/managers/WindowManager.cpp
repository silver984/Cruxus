#include <slv/game/managers/WindowManager.hpp>
#include <fmt/format.h>
#include <slv/engine/log.hpp>
#include <slv/internal/raylib.hpp>
#include <slv/game/managers/SceneManager.hpp>
#include <slv/game/managers/ResourceManager.hpp>
#ifdef _WIN32
#include <platforms/windows.hpp>
#endif
#include <raylib.h>
#include <algorithm>
#include <cstddef>
#include <cmath>

namespace slv {

WindowManager::WindowManager() :
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

WindowManager::~WindowManager() = default;

// private
bool WindowManager::init(
	std::string_view title,
	size<int> const& size,
	int fps,
	window_settings settings,
	context const& ctx
) {
	if (is_initialized_) {
		return true;
	}

	// disable raylib's logs
	SetTraceLogCallback([](int, char const*, va_list){});
	configure_settings(settings);

	title_ = std::string(title);
	default_screen_size_.width = std::max(1, size.width);
	default_screen_size_.height = std::max(1, size.height);
	target_fps_ = std::max(1, fps);

	if (
		!raylib::init_window(
			default_screen_size_.width,
			default_screen_size_.height,
			target_fps_,
			title_.c_str()
		)
	) {
		log::error("Failed to initialize window");
		return false;
	}

	if (!win32::enable_console_colors()) {
		log::warning("Couldn't enable console colors");
	}

	is_initialized_ = true;
	update(0.f, ctx);
	log::info("Window initialized");
	return true;
}

// private
void WindowManager::uninit() {
	if (!is_initialized_) {
		return;
	}

	is_initialized_ = false;

	log::info("Destroying window...");

	CloseWindow();
}

// private
void WindowManager::update(float dt, context const& ctx) {
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
	// a window automatically fullscreens the window when the size is the same as the monitor's and if the window position is (0, 0)
	// this disables that in case if the window is not supposed to be fullscreen
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

#if (defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)) && _WIN32
	memory_usage_ = win32::proc_memory_mb();
	max_memory_usage_ = std::max(memory_usage_, max_memory_usage_);
#endif
}

// private
void WindowManager::start_draw() const {
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
void WindowManager::end_draw() const {
	if (!is_initialized_) {
		return;
	}

#if defined(SLV_DEBUG) || defined(SLV_RELWITHDEBINFO)
	std::string debug_text = fmt::format(
		"FPS: {} / {:.0f}ms",
		running_fps(),
		delta_time() * 1000.f
	);

	if (memory_usage_ != 0.f) {
		debug_text += fmt::format(
			"\nMEM: {:.2f}mb / {:.2f}mb",
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

bool WindowManager::is_open() const {
	if (is_initialized_) {
		return !WindowShouldClose();
	}

	return false;
}

vec2<float> WindowManager::screen_center() const {
	float ui_scale_val = ui_scale();
	return vec2<float>(
		(draw_size_.width / 2.f) / ui_scale_val,
		(draw_size_.height / 2.f) / ui_scale_val
	);
}

size<float> WindowManager::screen_size() const {
	float ui_scale_val = ui_scale();
	return size<float>(
		draw_size_.width / ui_scale_val,
		draw_size_.height / ui_scale_val
	);
}

// TODO: fix window sizing

void WindowManager::set_size(size<int> const& size, bool set_as_default) {
	set_width(size.width, set_as_default);
	set_height(size.height, set_as_default);
}

void WindowManager::set_width(int width, bool set_as_default) {
	if (!is_initialized_) {
		return;
	}

	if (set_as_default) {
		default_screen_size_.width = std::max(1, width);
	}

	SetWindowSize(set_as_default ? default_screen_size_.width : std::max(1, width), default_screen_size_.height);
	was_resized_ = true;
}

void WindowManager::set_height(int height, bool set_as_default) {
	if (!is_initialized_) {
		return;
	}

	if (set_as_default) {
		default_screen_size_.height = std::max(1, height);
	}

	SetWindowSize(default_screen_size_.width, set_as_default ? default_screen_size_.height : std::max(1, height));
	was_resized_ = true;
}

void WindowManager::set_title(std::string_view title) {
	if (is_initialized_) {
		title_ = title;
		SetWindowTitle(title_.c_str());
	}
}

void WindowManager::set_fps(int fps) {
	if (is_initialized_) {
		target_fps_ = std::max(1, fps);
		SetTargetFPS(target_fps_);
	}
}

size<int> WindowManager::monitor_size() const {
	if (is_initialized_) {
		int monitor = GetCurrentMonitor();
		return size<int>(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	}

	return size<int>();
}

float WindowManager::delta_time() const {
	if (is_initialized_) {
		return GetFrameTime();
	}

	return 0.f;
}

vec2<float> WindowManager::mouse_pos() const {
	if (is_initialized_) {
		Vector2 pos = GetMousePosition();
		float ui_scale_val = ui_scale();
		return ui_scale_val != 0.f
			? vec2<float>(pos.x / ui_scale_val, pos.y / ui_scale_val)
			: vec2<float>(pos.x, pos.y);
	}

	return vec2<float>();
}

vec2<float> WindowManager::mouse_delta() const {
	if (is_initialized_) {
		Vector2 dt = GetMouseDelta();
		return vec2<float>(dt.x, dt.y);
	}

	return vec2<float>();
}

float WindowManager::ui_scale() const {
	float w = default_screen_size_.width > 0
		? static_cast<float>(draw_size_.width) / default_screen_size_.width
		: 1.f;

	float h = default_screen_size_.height > 0
		? static_cast<float>(draw_size_.height) / default_screen_size_.height
		: 1.f;
	
	return std::min(w, h);
}

void WindowManager::set_pos(const vec2<int>& pos) {
	if (is_initialized_ && !IsWindowFullscreen()) {
		pos_ = pos;
		SetWindowPosition(pos.x, pos.y);
	}
}

void WindowManager::set_pos_x(int x) {
	set_pos(vec2<int>(x, pos_.y));
}

void WindowManager::set_pos_y(int y) {
	set_pos(vec2<int>(pos_.x, y));
}

void WindowManager::toggle_fullscreen(bool val) {
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

bool WindowManager::is_fullscreen() const {
	if (is_initialized_) {
		return IsWindowFullscreen();
	}

	return false;
}

bool WindowManager::is_transparent() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_TRANSPARENT);
	}

	return false;
}

bool WindowManager::is_resizable() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_RESIZABLE);
	}

	return false;
}

bool WindowManager::is_borderless() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_WINDOW_UNDECORATED);
	}

	return false;
}

bool WindowManager::has_vsync() const {
	if (is_initialized_) {
		return IsWindowState(FLAG_VSYNC_HINT);
	}

	return false;
}

bool WindowManager::was_resized() const {
	return was_resized_;
}

int WindowManager::running_fps() const {
	return running_fps_;
}

size<int> WindowManager::draw_size() const {
	return draw_size_;
}

size<int> WindowManager::default_screen_size() const {
	return default_screen_size_;
}

std::string_view WindowManager::title() const {
	return title_;
}

int WindowManager::target_fps() const {
	return target_fps_;
}

vec2<int> WindowManager::pos() const {
	return pos_;
}

// private
void WindowManager::configure_settings(window_settings settings) {
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

} // namespace slv