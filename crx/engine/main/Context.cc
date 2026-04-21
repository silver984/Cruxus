#include <crx/engine/main/context.hh>

namespace crx {

context::context() :
	audio_sys_(nullptr),
	input_sys_(nullptr),
	resource_sys_(nullptr),
	director_(nullptr),
	window_(nullptr)
{}

context::context(
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

InputSys const* context::input_sys() const {
	return input_sys_;
}

AudioSys const* context::audio_sys() const {
	return audio_sys_;
}

ResourceSys const* context::resource_sys() const {
	return resource_sys_;
}

Director const* context::director() const {
	return director_;
}

Window const* context::window() const {
	return window_;
}

}