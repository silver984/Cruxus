#pragma once
#include <slv/types/memory.hpp>
#include <vector>
#include <cstdint>

namespace slv {

struct audio_hnd final {
	sptr<std::vector<float>> pcm = nullptr;
	uint64_t cursor = 0;
};

}