#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/util/pointers.hh>
#include <cruxus/engine/main/Context.hh>
#include <vector>
#include <string_view>

namespace crx {

class CRX_DLL Sound {
private:
	explicit Sound(sptr<std::vector<float>> pcm);

public:
	Sound();
	~Sound();

	static Sound create(Context const& ctx, std::string_view file_path);
	void play(Context const& ctx) const;
	void set_volume(float val);
	[[nodiscard]] float volume() const;

private:
	float volume_;
	sptr<std::vector<float>> pcm_;
};

}