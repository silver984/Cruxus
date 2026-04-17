#pragma once
#include <slv/engine/Game/managers/Harbor.hpp>
#include <slv/engine/log.hpp>

namespace crx {

// private
Harbor::Harbor() :
	is_destroying_current_scene_(false)
{};

// private
Harbor::~Harbor() = default;

void Harbor::change_scene(sptr<Vessel>&& new_scene) {
	if (!new_scene) {
		log::error("The scene the Game tried to change into is nullptr");
		return;
	}

	pending_scene_ = std::move(new_scene);
}

void Harbor::destroy_current_scene() {
	is_destroying_current_scene_ = true;
}

wptr<Vessel> Harbor::current_scene() {
	return current_scene_;
}

// private
void Harbor::update(context const& ctx, float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
	}

	if (is_destroying_current_scene_) {
		safely_destroy_scene();
		is_destroying_current_scene_ = false;
	}

	if (current_scene_) {
		current_scene_->base_update(ctx, dt);
	}
}

// private
void Harbor::draw(context const& ctx) {
	if (current_scene_) {
		current_scene_->base_draw(ctx);
	}
}

// private
void Harbor::safely_destroy_scene() {
	if (current_scene_) {
		current_scene_->destroy();
		current_scene_.reset();
		current_scene_ = nullptr;
	}
}

}