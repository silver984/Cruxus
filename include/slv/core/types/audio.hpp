#pragma once

#include <miniaudio/miniaudio.h>

namespace slv
{
	struct audio_hnd
	{
		ma_decoder decoder{};
		bool active = false;
		float volume = 1.0f;
	};
}