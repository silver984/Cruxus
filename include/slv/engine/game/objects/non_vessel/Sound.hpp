#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/audio.hpp>
#include <slv/types/context.hpp>
#include <string_view>

namespace slv {

class SLV_DLL Sound {
public:
	Sound();
	~Sound();

	bool create(context const& ctx, std::string_view file_path);
	void play(context const& ctx);

private:
	sptr<pcm_data> pcm_data_;
};

}