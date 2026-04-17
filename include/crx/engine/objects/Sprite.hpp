#pragma once
#include <crx/config.hh>
#include <crx/engine/Game/objects/Vessel.hpp>
#include <crx/types/pointers.hpp>
#include <crx/types/rect.hpp>
#include <crx/types/graphic.hpp>
#include <crx/types/context.hpp>
#include <string>
#include <string_view>

namespace crx {

class CRX_DLL Sprite : public Vessel {
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
	sptr<texture> texture_;
};

}