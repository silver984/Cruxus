#include <crx/engine/main/Game.hh>
#include <crx/engine/debug/log.hh>
#include <fmt/format.h>
#include <chrono>

namespace crx {

Game::Game() :
	is_initialized_(false)
{}

Game::~Game() {
	shutdown();
}

bool Game::init(
	std::string_view win_title,
	size<int> const& win_size,
	int win_fps,
	Window::config& win_settings
) {
	if (is_initialized_) {
		return true;
	}

	if (
		!window_.init(
			win_title,
			win_size,
			win_fps,
			win_settings,
			ctx()
		)
	) {
		log::error("Failed to initialize Window");
		return false;
	}

	if (!audio_sys_.init(&window_)) {
		log::warning("Failed to initialize audio");
	}

	/*if (!crash_.init()) {
		log::warning("Failed to initialize the crash manager, it may be unsupported on this platform");
	}*/

	is_initialized_ = true;

	log::info("Initialized");

	return true;
}

void Game::run() {
	if (!is_initialized_) {
		return;
	}

	auto ctx_val = ctx();

	while (window_.is_open()) {
		float dt = window_.delta_time();
		window_.update(ctx_val, dt);
		input_sys_.update(dt);
		resource_sys_.update(dt);
		director_.update(ctx_val, dt);

		window_.start_draw();
		director_.draw(ctx_val);
		window_.end_draw();
	}

	shutdown();
}

context Game::ctx() {
	return context(
		&audio_sys_,
		&input_sys_,
		&resource_sys_,
		&director_,
		&window_
	);
}

// private
void Game::shutdown() {
	if (!is_initialized_) {
		return;
	}

	log::debug("Shutting down...");

	auto start = std::chrono::high_resolution_clock::now();

	director_.safely_destroy_scene();
	resource_sys_.clean_cache();
	audio_sys_.shutdown();
	window_.shutdown();
	is_initialized_ = false;

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	log::debug(fmt::format("Took {}ms", elapsed.count()));
}

}