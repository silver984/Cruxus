#pragma once
#include <slv/types/pointers.hpp>
#include <vector>
#include <cstdint>

namespace slv {

struct audio final {
	audio();
	sptr<std::vector<float>> pcm;
	uint32_t cursor;
	float volume;
};

}