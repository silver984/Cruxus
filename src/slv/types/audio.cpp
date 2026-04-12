#include <slv/types/audio.hpp>

namespace slv {

audio_hnd::audio_hnd() :
	pcm(nullptr),
	cursor(0)
{}

audio_hnd::audio_hnd(sptr<pcm_data> pcm_val, uint64_t cursor_val) :
	pcm(pcm_val),
	cursor(cursor_val)
{}

}