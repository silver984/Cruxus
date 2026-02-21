#pragma once

#include <vector>
#include <slv/core/types/pointers.hpp>
#include <cstdint>

namespace slv
{
	using pcm_data = std::vector<float>;
	
	struct audio_hnd
	{
		slv::sptr<pcm_data> pcm = nullptr;
		uint64_t cursor = 0;
	};
}