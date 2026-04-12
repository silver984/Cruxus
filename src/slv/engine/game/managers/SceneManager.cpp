#pragma once
#include <slv/engine/game/managers/SceneManager.hpp>
#include <slv/engine/log.hpp>

namespace slv {

// private
SceneManager::SceneManager() :
	is_destroying_current_scene_(false)
{};

// private
SceneManager::~SceneManager() = default;

void SceneManager::change_scene(sptr<Vessel>&& new_scene) {
	if (!new_scene) {
		log::error("The scene the game tried to change into is nullptr");
		return;
	}

	pending_scene_ = std::move(new_scene);
}

void SceneManager::destroy_current_scene() {
	is_destroying_current_scene_ = true;
}

wptr<Vessel> SceneManager::current_scene() {
	return current_scene_;
}

// private
void SceneManager::update(float dt, context const& ctx) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
	}

	if (is_destroying_current_scene_) {
		safely_destroy_scene();
		is_destroying_current_scene_ = false;
	}

	if (current_scene_) {
		current_scene_->base_update(dt, ctx);
	}
}

// private
void SceneManager::draw(context const& ctx) {
	if (current_scene_) {
		current_scene_->base_draw(ctx);
	}
}

// private
void SceneManager::safely_destroy_scene() {
	if (current_scene_) {
		current_scene_->destroy();
		current_scene_.reset();
	}
}

}