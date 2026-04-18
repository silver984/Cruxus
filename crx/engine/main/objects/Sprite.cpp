#include <crx/engine/Game/objects/Sprite.hpp>
#include <crx/engine/main/backend/ResourceSys.hpp>
#include "rl.hpp"

namespace crx {

Sprite::Sprite(std::string_view texture_file_path) :
	texture_file_path_(texture_file_path),
	texture_(nullptr)
{}

Sprite::~Sprite() = default;

std::string_view Sprite::type() const {
	static constexpr std::string_view TYPE = "Sprite";
	return TYPE;
}

void Sprite::set_antialiasing(bool val) {
	if (texture_) {
		rl::set_texture_antialiasing(*texture_, val);
	}
}

// protected
bool Sprite::init(context const& ctx) {
	auto& resource = ctx.resource;

	if (!resource) {
		return false;
	}

	texture_ = resource->load_texture(texture_file_path_);

	if (!texture_) {
		return false;
	}

	bounds_ = size<float>(
		static_cast<float>(texture_->bounds.width),
		static_cast<float>(texture_->bounds.height)
	);

	source_rect_ = rect<float>(
		0.f, 0.f,
		bounds_.width,
		bounds_.height
	);

	set_antialiasing(true);
	update(ctx, 0.f);

	return true;
}

// protected
void Sprite::draw(context const& ctx) const {
	if (
		texture_ &&
		source_rect_.bounds.width > 0.f &&
		source_rect_.bounds.height > 0.f
	) {
		rl::draw_texture(
			*texture_,
			source_rect_,
			vec2<float>(0.f, 0.f),
			world_transform(),
			color,
			world_alpha()
		);
	}
}

}