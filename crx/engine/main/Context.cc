#include <crx/engine/main/Context.hh>

namespace crx {
// private
Context::Context() :
	audio_sys_(nullptr),
	input_sys_(nullptr),
	resource_sys_(nullptr),
	window_(nullptr),
	director_(nullptr)
{}

// private
Context::Context(
	AudioSys* audio_sys,
	InputSys* input_sys,
	ResourceSys* resource_sys,
	Director* director,
	Window* window
) :
	audio_sys_(audio_sys),
	input_sys_(input_sys),
	resource_sys_(resource_sys),
	window_(window),
	director_(director)
{}

// private
Context::~Context() = default;

InputSys* const& Context::input_sys() {
	return input_sys_;
}

AudioSys* const& Context::audio_sys() {
	return audio_sys_;
}

ResourceSys* const& Context::resource_sys() {
	return resource_sys_;
}

Director* const& Context::director() {
	return director_;
}

Window* const& Context::window() {
	return window_;
}
}