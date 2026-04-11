#include <slv/game/Game.hpp>
#include <slv/engine/log.hpp>

namespace slv {
Game::Game() :
	is_initialized_(false)
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

	if (!window_.init(win_title, win_size, win_fps, win_settings, ctx())) {
		return false;
	}

	audio_.init();
	crash_.init();

	is_initialized_ = true;

	return true;
}

void Game::run() {
	auto ctx_val = ctx();

	while (window_.is_open()) {
		// update

		float dt = window_.delta_time();
		window_.update(dt, ctx_val);
		input_.update(dt);
		resource_.update(dt);
		scene_.update(dt, ctx_val);

		// draw

		window_.start_draw();
		scene_.draw(ctx_val);
		window_.end_draw();
	}

	// cleanup, close window

	scene_.safely_destroy_scene();
	input_.clean_cache();
	resource_.clean_cache();
	audio_.uninit();
	window_.uninit();
}

context Game::ctx() {
	return {
		&window_,
		&scene_,
		&input_,
		&audio_,
		&resource_
	};
}

}