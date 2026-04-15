#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/context.hpp>
#include <vector>
#include <string_view>

namespace slv {

class SLV_DLL Sound {
private:
	explicit Sound(sptr<std::vector<float>> pcm);

public:
	Sound();
	~Sound();

	static Sound create(context const& ctx, std::string_view file_path);
	void play(context const& ctx) const;
	void set_volume(float val);
	[[nodiscard]] float volume() const;

private:
	float volume_;
	sptr<std::vector<float>> pcm_;
};

}