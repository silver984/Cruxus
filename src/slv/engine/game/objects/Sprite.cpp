#include <slv/engine/game/objects/Sprite.hpp>
#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/internal/raylib.hpp>

namespace slv {

Sprite::Sprite(std::string_view file_path) :
	file_path_(file_path),
	texture_(nullptr)
{}

Sprite::~Sprite() = default;

std::string_view Sprite::type() const {
	return "Sprite";
}

void Sprite::set_antialiasing(bool val) {
	if (texture_) {
		raylib::set_texture_antialiasing(*texture_, val);
	}
}

// protected
bool Sprite::init(context const& ctx) {
	auto& resource = ctx.resource;

	if (!resource) {
		return false;
	}

	texture_ = resource->load_texture(file_path_);

	if (!texture_) {
		return false;
	}

	content_size_ = size<float>(
		static_cast<float>(texture_->resolution.width),
		static_cast<float>(texture_->resolution.height)
	);

	source_ = rect<float>(
		0.f, 0.f,
		content_size_.width,
		content_size_.height
	);

	set_antialiasing(true);
	update(0.f, ctx);

	return true;
}

// protected
void Sprite::draw(context const& ctx) const {
	if (
		texture_ &&
		source_.dimensions.width > 0.f &&
		source_.dimensions.height > 0.f
	) {
		raylib::draw_texture(
			*texture_,
			source_,
			vec2<float>(0.f, 0.f),
			world_transform(),
			color,
			world_alpha()
		);
	}
}

}