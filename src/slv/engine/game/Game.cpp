#include <slv/engine/game/Game.hpp>
#include <slv/engine/log.hpp>
#include <fmt/format.h>

namespace slv {

Game::Game() :
	is_initialized_(false),
	is_window_minimized_(false)
{}

Game::~Game() = default;

bool Game::init(
	std::string_view win_title,
	size<int> const& win_size,
	int win_fps,
	window_settings win_settings
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
			get_ctx()
		)
	) {
		log::error("Failed to initialize window");
		return false;
	}

	if (!audio_.init(&window_)) {
		log::warning("Failed to initialize audio");
	}

	if (!crash_.init()) {
		log::warning("Failed to initialize the crash manager, it may be unsupported on this platform");
	}

	is_initialized_ = true;

	return true;
}

void Game::run() {
	if (!is_initialized_) {
		return;
	}

	auto ctx = get_ctx();

	while (window_.is_open()) {
		// update
		float dt = window_.delta_time();
		window_.update(ctx, dt);
		input_.update(dt);
		resource_.update(dt);
		scene_.update(ctx, dt);

		// draw
		window_.start_draw();
		scene_.draw(ctx);
		window_.end_draw();
	}

	scene_.safely_destroy_scene();
	resource_.clean_cache();
	audio_.uninit();
	window_.uninit();
}

context Game::get_ctx() {
	return context(
		&window_,
		&scene_,
		&input_,
		&audio_,
		&resource_
	);
}

}