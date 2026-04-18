#pragma once
#include <crx/config.hh>
#include <crx/engine/main/objects/Node.hh>
#include <crx/engine/math/size.hh>
#include <crx/engine/gfx/rgb.hh>
#include <crx/engine/main/context.hh>
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
	bool init(context const& ctx) override;
	void draw(context const& ctx) const override;
};
}