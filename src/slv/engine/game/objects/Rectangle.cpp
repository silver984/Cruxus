#include <slv/engine/game/objects/rectangle.hpp>
#include <slv/internal/raylib.hpp>

namespace slv {

Rectangle::Rectangle(size<float> const& size, rgb const& color_val) {
	content_size_ = size;
	color = color_val;
}

Rectangle::~Rectangle() = default;

std::string_view Rectangle::type() const {
	static constexpr std::string_view TYPE = "Rectangle";
	return TYPE;
}

void Rectangle::set_content_size(size<float> const& val) {
	content_size_ = val;
}

void Rectangle::set_width(float width) {
	content_size_.width = width;
}

void Rectangle::set_height(float height) {
	content_size_.height = height;
}

// protected
bool Rectangle::init(context const& ctx) {
	update(ctx, 0.f);
	return true;
}

// protected
void Rectangle::draw(context const& ctx) const {
	raylib::draw_rectangle(world_transform(), content_size_, color, world_alpha());
}

}