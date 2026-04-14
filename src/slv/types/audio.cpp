#include <slv/types/audio.hpp>

namespace slv {

audio::audio() :
	pcm(nullptr),
	cursor(0),
	volume(1.f)
{}

}