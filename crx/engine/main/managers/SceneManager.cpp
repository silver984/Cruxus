#pragma once
#include <crx/engine/main/backend/Director.hpp>
#include <crx/engine/log.hpp>

namespace crx {

// private
Director::Director() :
	is_destroying_current_scene_(false)
{};

// private
Director::~Director() = default;

void Director::change_scene(sptr<Node>&& new_scene) {
	if (!new_scene) {
		log::error("The scene the Game tried to change into is nullptr");
		return;
	}

	pending_scene_ = std::move(new_scene);
}

void Director::destroy_current_scene() {
	is_destroying_current_scene_ = true;
}

wptr<Node> Director::current_scene() {
	return current_scene_;
}

// private
void Director::update(context const& ctx, float dt) {
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
void Director::draw(context const& ctx) {
	if (current_scene_) {
		current_scene_->base_draw(ctx);
	}
}

// private
void Director::safely_destroy_scene() {
	if (current_scene_) {
		current_scene_->destroy();
		current_scene_.reset();
		current_scene_ = nullptr;
	}
}

}