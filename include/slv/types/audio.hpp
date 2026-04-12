#pragma once
#include <slv/types/pointers.hpp>
#include <vector>
#include <cstdint>

namespace slv {

using pcm_data = std::vector<float>;

struct audio_hnd final {
	audio_hnd();
	explicit audio_hnd(sptr<pcm_data> pcm_val, uint64_t cursor_val);
	sptr<pcm_data> pcm;
	uint64_t cursor;
};

}