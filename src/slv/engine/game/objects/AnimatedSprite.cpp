#include <slv/engine/game/objects/AnimatedSprite.hpp>
#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/internal/raylib.hpp>
#include <slv/engine/log.hpp>
#include <fmt/format.h>
#include <algorithm>

namespace slv {

AnimatedSprite::AnimatedSprite(std::string_view texture_file_path, std::string_view data_file_path) :
	Sprite(texture_file_path),
	fps_(0.f),
	frame_elapsed_(0.f),
	is_looping_(false),
	cur_frame_index_(0),
	atlas_data_(nullptr),
	data_file_path_(data_file_path)
{}

AnimatedSprite::~AnimatedSprite() = default;

void AnimatedSprite::add_anim_alias(std::string_view alias, std::string_view anim_name) {
	if (!atlas_data_) {
		return;
	}

	if (!atlas_data_->frames.contains(alias)) {
		return;
	}

	if (auto it = aliases_.find(alias); it == aliases_.end()) {
		// create it
		aliases_.emplace(alias, anim_name);
	} else {
		it->second = anim_name;
	}
}

void AnimatedSprite::play_anim(std::string_view name, bool is_looping) {
	if (!atlas_data_) {
		return;
	}

	std::string_view resolved = name;

	if (auto it = aliases_.find(name); it != aliases_.end()) {
		resolved = it->second;
	}

	if (!atlas_data_->frames.contains(resolved)) {
		return;
	}

	cur_anim_ = std::string(resolved);
	is_looping_ = is_looping;
	cur_frame_index_ = 0;
}

void AnimatedSprite::set_anim_offsets(std::string_view name, vec2<float> const& offsets) {
	std::string_view resolved = name;

	if (auto it = aliases_.find(resolved); it != aliases_.end()) {
		resolved = it->second;
	}

	if (auto it = offsets_.find(resolved); it != offsets_.end()) {
		it->second = offsets;
	}
}

void AnimatedSprite::set_fps(float fps) {
	fps_ = fps;
}

std::string_view AnimatedSprite::type() const {
	return "AnimatedSprite";
}

std::string_view AnimatedSprite::cur_anim() const {
	return cur_anim_;
}

// protected
bool AnimatedSprite::init(context const& ctx) {
	auto& resource = ctx.resource;

	if (!resource) {
		return false;
	}

	texture_ = resource->load_texture(texture_file_path_);
	atlas_data_ = resource->load_atlas_data(data_file_path_);

	if (!texture_ || !atlas_data_) {
		return false;
	}

	content_size_ = avg_frame_size(atlas_data_);

	if (!atlas_data_->frames.empty()) {
		cur_anim_ = atlas_data_->frames.begin()->first;
	}

	for (const auto& [name, _] : atlas_data_->frames) {
		offsets_.emplace(name, vec2<float>(0.f, 0.f));
	}

	set_antialiasing(true);
	update(ctx, 0.f);

	return true;
}

// protected
void AnimatedSprite::update(context const& ctx, float dt) {
	if (!atlas_data_) {
		return;
	}

	auto it = atlas_data_->frames.find(cur_anim_);
	if (it == atlas_data_->frames.end() || it->second.empty()) {
		return;
	}

	const auto& cur_frames = it->second;

	if (fps_ > 0.f) {
		frame_elapsed_ += dt;
		float target_dt = 1.f / fps_;

		while (frame_elapsed_ >= target_dt) {
			if (is_looping_) {
				cur_frame_index_ = (cur_frame_index_ + 1) % cur_frames.size();
			} else {
				cur_frame_index_ = std::min(cur_frame_index_ + 1, cur_frames.size() - 1);
			}

			frame_elapsed_ -= target_dt;
		}
	}

	const auto& cur_frame = cur_frames[cur_frame_index_];

	source_ = rect<float>(
		cur_frame.pos_on_sheet.x,
		cur_frame.pos_on_sheet.y,
		cur_frame.size_on_sheet.width,
		cur_frame.size_on_sheet.height
	);

	auto atlas_offsets = vec2<float>(
		cur_frame.offsets.x,
		cur_frame.offsets.y
	);

	cur_offsets_ = offsets_[cur_anim_] - atlas_offsets;
}

// protected
void AnimatedSprite::draw(context const& ctx) const {
	if (
		texture_ &&
		source_.dimensions.width > 0.f &&
		source_.dimensions.height > 0.f
	) {
		raylib::draw_texture(
			*texture_,
			source_,
			cur_offsets_,
			world_transform(),
			color,
			world_alpha()
		);
	}
}

// private
size<float> AnimatedSprite::avg_frame_size(sptr<atlas_data> data) {
	if (!data) {
		return {};
	}

	size<float> compounded_size;
	size_t total = 0;
	
	for (auto& [_, frames] : data->frames) {
		for (const auto& frame : frames) {
			compounded_size.width += frame.size_on_sheet.width;
			compounded_size.height += frame.size_on_sheet.height;
			++total;
		}
	}

	if (total != 0) {
		return compounded_size / total;
	}

	return {};
}

}