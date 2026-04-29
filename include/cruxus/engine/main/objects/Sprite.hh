#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/main/objects/Node.hh>
#include <cruxus/engine/util/pointers.hh>
#include <cruxus/engine/math/rect.hh>
#include <cruxus/engine/gfx/texture.hh>
#include <cruxus/engine/main/Context.hh>
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
	bool init(Context const& ctx) override;
	void draw(Context const& ctx) const override;

	std::string texture_file_path_;
	rect<float> source_rect_;
	sptr<texture_atlas> texture_atlas_;
};

}