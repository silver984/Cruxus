#pragma once
#include <crx/engine/main/Director.hh>
#include <crx/engine/debug/log.hh>

namespace crx {
// private
Director::Director() :
	is_destroying_current_scene_(false)
{};

// private
Director::~Director() = default;

void Director::switch_scene(sptr<Node>&& new_scene) {
	if (!new_scene) {
		log::error("Attempted to switch to a nullptr scene");
		return;
	}

	pending_scene_ = std::move(new_scene);
	log::debug("New scene pending");
}

void Director::destroy_current_scene() {
	is_destroying_current_scene_ = true;
	log::debug("Called to destroy current scene");
}

wptr<Node> Director::current_scene() {
	return current_scene_;
}

// private
void Director::update(context const& ctx, float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
		if (current_scene_) {
			log::info("Successfully switched scenes");
		}
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
	if (!current_scene_) {
		return;
	}

	current_scene_->destroy();
	current_scene_.reset();
	current_scene_ = nullptr;
	log::info("Scene destroyed");
}
}