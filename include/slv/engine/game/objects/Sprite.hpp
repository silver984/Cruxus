#pragma once
#include <slv/config.hpp>
#include <slv/engine/game/objects/Vessel.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/rect.hpp>
#include <slv/types/graphic.hpp>
#include <slv/types/context.hpp>
#include <string>
#include <string_view>

namespace slv {

class SLV_DLL Sprite : public Vessel {
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