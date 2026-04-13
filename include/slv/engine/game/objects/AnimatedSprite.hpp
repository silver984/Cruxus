#pragma once
#include <slv/internal/config.hpp>
#include <slv/engine/game/objects/Sprite.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/primitives.hpp>
#include <slv/types/graphic.hpp>
#include <slv/types/string_map.hpp>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace slv {

class SLV_DLL AnimatedSprite : public Sprite {
public:
	AnimatedSprite(std::string_view texture_file_path, std::string_view data_file_path);
	~AnimatedSprite() override;

	void add_anim_alias(std::string_view alias, std::string_view anim_name);
	void play_anim(std::string_view name, bool is_looping = true);
	void set_anim_offsets(std::string_view name, vec2<float> const& offsets);
	void set_fps(float fps);
	[[nodiscard]] std::string_view type() const override;
	[[nodiscard]] std::string_view cur_anim() const;

	/*
	to implement later
	std::string get_current_anim_alias();
	float get_current_anim_fps();
	bool is_current_anim_looping();
	bool is_current_anim_playing();
	*/

protected:
	bool init(context const& ctx) override;
	void update(context const& ctx, float dt) override;
	void draw(context const& ctx) const override;

	float fps_;
	bool is_looping_;
	std::string data_file_path_;
	sptr<atlas_data> atlas_data_;
	std::string cur_anim_;

private:
	[[nodiscard]] size<float> avg_frame_size(sptr<atlas_data> data);

	float frame_elapsed_;
	size_t cur_frame_index_;
	vec2<float> cur_offsets_;
	string_map<std::string> aliases_;
	string_map<vec2<float>> offsets_;
};

}