#pragma once
#include <crx/config.hh>
#include <crx/engine/main/objects/Node.hh>
#include <crx/engine/util/pointers.hh>
#include <crx/engine/math/rect.hh>
#include <crx/engine/gfx/texture.hh>
#include <crx/engine/main/context.hh>
#include <string>
#include <string_view>

namespace crx {

class CRX_DLL Sprite : public Node {
public:
	Sprite(std::string_view texture_file_path);
	~Sprite() override;

	[[nodiscard]] std::string_view type() const override;
	void set_antialiasing(bool val);

protected:
	bool init(context const& ctx) override;
	void draw(context const& ctx) const override;

	std::string texture_file_path_;
	rect<float> source_rect_;
	sptr<texture_atlas> texture_atlas_;
};

}