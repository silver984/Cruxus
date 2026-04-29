#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/main/objects/Node.hh>
#include <cruxus/engine/math/size.hh>
#include <cruxus/engine/gfx/rgb.hh>
#include <cruxus/engine/main/Context.hh>
#include <string_view>

namespace crx {

class CRX_DLL Rectangle : public Node {
public:
	Rectangle(size<float> const& bounds, rgb const& color_val);
	~Rectangle() override;

	[[nodiscard]] std::string_view type() const override;
	void set_content_size(size<float> const& val);
	void set_width(float width);
	void set_height(float height);

protected:
	bool init(Context const& ctx) override;
	void draw(Context const& ctx) const override;
};
}