#include <cruxus/engine/main/Context.hh>

namespace crx {

Context::Context() :
	audio_sys_(nullptr),
	input_sys_(nullptr),
	resource_sys_(nullptr),
	director_(nullptr),
	window_(nullptr)
{}

Context::Context(
	AudioSys* a,
	InputSys* i,
	ResourceSys* r,
	Director* d,
	Window* w
) :
	audio_sys_(a),
	input_sys_(i),
	resource_sys_(r),
	director_(d),
	window_(w)
{}

InputSys const* Context::input_sys() const {
	return input_sys_;
}

AudioSys const* Context::audio_sys() const {
	return audio_sys_;
}

ResourceSys const* Context::resource_sys() const {
	return resource_sys_;
}

Director const* Context::director() const {
	return director_;
}

Window const* Context::window() const {
	return window_;
}

}