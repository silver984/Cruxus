#include <crx/engine/Game/objects/rectangle.hh>
#include "rl.hh"

namespace crx {

Rectangle::Rectangle(size<float> const& bounds, rgb const& color_val) {
	bounds_ = bounds;
	color = color_val;
}

Rectangle::~Rectangle() = default;

std::string_view Rectangle::type() const {
	static constexpr std::string_view TYPE = "Rectangle";
	return TYPE;
}

void Rectangle::set_content_size(size<float> const& val) {
	bounds_ = val;
}

void Rectangle::set_width(float width) {
	bounds_.width = width;
}

void Rectangle::set_height(float height) {
	bounds_.height = height;
}

// protected
bool Rectangle::init(context const& ctx) {
	update(ctx, 0.f);
	return true;
}

// protected
void Rectangle::draw(context const& ctx) const {
	rl::draw_rectangle(world_transform(), bounds_, color, world_alpha());
}

}