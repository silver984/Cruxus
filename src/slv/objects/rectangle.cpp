#include <slv/objects/rectangle.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/math.hpp>
#include <algorithm>

namespace slv
{
	// protected
	void Rectangle::draw(const slv::game_context& ctx) const
	{
		slv::raylib::draw_rectangle(this->world_transform_, this->dimensions_, this->color, this->world_alpha_);
		
		// draw outline
		float line_thickness = std::max(0.f, outline_thickness);
		float line_alpha = std::clamp(outline_alpha, 0.f, 1.f) * this->world_alpha_;
		if (line_thickness > 0.f && line_alpha > 0.f)
		{
			slv::raylib::draw_rectangle_sharp_lines(this->world_transform_, this->dimensions_, this->outline_color, line_alpha, line_thickness);
		}
	}
}