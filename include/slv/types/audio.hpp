#pragma once
#include <slv/types/memory.hpp>
#include <vector>
#include <cstdint>

namespace slv {

using pcm_data = std::vector<float>;

struct audio_hnd final {
	sptr<pcm_data> pcm = nullptr;
	uint64_t cursor = 0;
};

}