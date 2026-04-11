#pragma once

#include <cstdint>

#ifndef SLV_AUDIO_SAMPLEFORMAT
#define SLV_AUDIO_SAMPLEFORMAT ma_format_f32
#endif

namespace slv
{
	constexpr inline uint32_t AUDIO_CHANNELS = 2;
	constexpr inline uint32_t AUDIO_SAMPLE_RATE = 48000;
}