#pragma once
#include <slv/config.hpp>
#include <slv/engine/game/objects/Vessel.hpp>
#include <slv/types/size.hpp>
#include <slv/types/rgb.hpp>
#include <slv/types/context.hpp>
#include <string_view>

namespace slv {

class SLV_DLL Rectangle : public Vessel {
public:
	Rectangle(size<float> const& bounds, rgb const& color_val);
	~Rectangle() override;

	[[nodiscard]] std::string_view type() const override;
	void set_content_size(size<float> const& val);
	void set_width(float width);
	void set_height(float height);

protected:
	bool init(context const& ctx) override;
	void draw(context const& ctx) const override;
};
}