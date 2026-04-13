#include <slv/engine/game/objects/AnimatedSprite.hpp>
#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/internal/raylib.hpp>
#include <slv/engine/log.hpp>
#include <fmt/format.h>
#include <algorithm>

namespace slv {

AnimatedSprite::AnimatedSprite(std::string_view texture_file_path, std::string_view data_file_path, float fps) :
	Sprite(texture_file_path),
	fps_(std::max(0.f, fps)),
	frame_elapsed_(0.f),
	is_looping_(false),
	cur_frame_index_(0),
	atlas_data_(nullptr),
	data_file_path_(data_file_path),
	cur_frames_(nullptr)
{}

AnimatedSprite::~AnimatedSprite() = default;

void AnimatedSprite::add_anim_alias(std::string_view alias, std::string_view anim_name) {
	if (!atlas_data_) {
		return;
	}

	if (!atlas_data_->frames.contains(anim_name)) {
		log::warning(fmt::format("\"{}\" is not found", anim_name));
		return;
	}

	if (auto it = aliases_.find(alias); it == aliases_.end()) {
		// create it if missing
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

	if (
		auto it = atlas_data_->frames.find(resolved);
		it == atlas_data_->frames.end()
	) {
		log::warning(fmt::format("\"{}\" is not found", resolved));
		return;
	} else {
		cur_frames_ = &it->second;
	}

	cur_anim_ = std::string(resolved);
	is_looping_ = is_looping;
	cur_frame_index_ = 0;
	frame_elapsed_ = 0.f;
}

void AnimatedSprite::set_anim_offsets(std::string_view name, vec2<float> const& offsets) {
	std::string_view resolved = name;

	if (auto it = aliases_.find(resolved); it != aliases_.end()) {
		resolved = it->second;
	}

	if (auto it = offsets_.find(resolved); it == offsets_.end()) {
		// create it if missing
		offsets_.emplace(resolved, offsets);
	} else {
		it->second = offsets;
	}
}

void AnimatedSprite::set_fps(float fps) {
	fps_ = std::max(0.f, fps);
}

float AnimatedSprite::fps() const {
	return fps_;
}

bool AnimatedSprite::is_looping() const {
	return is_looping_;
}

size_t AnimatedSprite::cur_frame_index() const {
	return cur_frame_index_;
}

std::string_view AnimatedSprite::type() const {
	static constexpr std::string_view TYPE = "AnimatedSprite";
	return TYPE;
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

	for (const auto& [name, _] : atlas_data_->frames) {
		offsets_.emplace(name, vec2<float>(0.f, 0.f));
	}

	set_antialiasing(true);
	update(ctx, 0.f);

	return true;
}

// protected
void AnimatedSprite::update(context const& ctx, float dt) {
	if (!cur_frames_ || cur_frames_->empty()) {
		return;
	}

	if (fps_ > 0.f) {
		frame_elapsed_ += dt;
		float target_dt = 1.f / fps_;

		while (frame_elapsed_ >= target_dt) {
			if (is_looping_) {
				cur_frame_index_ = (cur_frame_index_ + 1) % cur_frames_->size();
			} else {
				cur_frame_index_ = std::min(cur_frame_index_ + 1, cur_frames_->size() - 1);
			}

			frame_elapsed_ -= target_dt;
		}
	}

	const auto& cur_frame = (*cur_frames_)[cur_frame_index_];

	source_rect_ = rect<float>(
		cur_frame.pos_on_sheet.x,
		cur_frame.pos_on_sheet.y,
		cur_frame.size_on_sheet.width,
		cur_frame.size_on_sheet.height
	);

	auto frame_offsets = vec2<float>(
		cur_frame.offsets.x,
		cur_frame.offsets.y
	);

	cur_offsets_ = offsets_.at(cur_anim_) - frame_offsets;
}

// protected
void AnimatedSprite::draw(context const& ctx) const {
	if (
		texture_ &&
		source_rect_.dimensions.width > 0.f &&
		source_rect_.dimensions.height > 0.f
	) {
		raylib::draw_texture(
			*texture_,
			source_rect_,
			cur_offsets_,
			world_transform(),
			color,
			world_alpha()
		);
	}
}

// private
size<float> AnimatedSprite::avg_frame_size(sptr<atlas_data> const& data) {
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