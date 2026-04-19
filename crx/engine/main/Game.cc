#include <crx/engine/main/Game.hh>
#include <crx/engine/debug/log.hh>
#include <fmt/format.h>
#include <chrono>

namespace crx {

Game::Game() :
	is_initialized_(false),
	is_window_minimized_(false)
{}

Game::~Game() = default;

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

	if (!audio_.init(&window_)) {
		log::warning("Failed to initialize audio");
	}

	/*if (!crash_.init()) {
		log::warning("Failed to initialize the crash manager, it may be unsupported on this platform");
	}*/

	is_initialized_ = true;

	log::info("Game initialized");

	return true;
}

void Game::run() {
	if (!is_initialized_) {
		return;
	}

	auto _ctx_ = ctx();

	while (window_.is_open()) {
		// update
		{
			// auto start = std::chrono::high_resolution_clock::now();
			
			float dt = window_.delta_time();
			window_.update(_ctx_, dt);
			input_.update(dt);
			resource_.update(dt);
			director_.update(_ctx_, dt);

			/*auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			debug_.push_update_cpu_time(elapsed.count());*/
		}

		// draw
		{
			// auto start = std::chrono::high_resolution_clock::now();

			window_.start_draw();
			director_.draw(_ctx_);
			window_.end_draw();
			
			/*auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = end - start;
			debug_.push_draw_cpu_time(elapsed.count());*/
		}
	}

	log::debug("Cleaning up...");
	
	auto start = std::chrono::high_resolution_clock::now();

	director_.safely_destroy_scene();
	resource_.clean_cache();
	audio_.uninit();
	window_.uninit();

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	log::debug(fmt::format("Took {}ms", elapsed.count()));
}

context Game::ctx() {
	return context(
		&audio_,
		&input_,
		&resource_,
		&director_,
		&window_
	);
}

}