#pragma once
#include <crx/config.hh>

namespace crx {

class AudioSys;
class InputSys;
class ResourceSys;
class Director;
class Window;
class Game;

struct CRX_DLL context final {
	friend class Game;
private:
	context();
	explicit context(
		AudioSys* a,
		InputSys* i,
		ResourceSys* r,
		Director* d,
		Window* w
	);

public:
	context(context const&) = delete;
	context(context&&) = delete;
	context& operator =(context const&) = delete;
	context& operator =(context&&) = delete;

	[[nodiscard]] InputSys const* input_sys() const;
	[[nodiscard]] AudioSys const* audio_sys() const;
	[[nodiscard]] ResourceSys const* resource_sys() const;
	[[nodiscard]] Director const* director() const;
	[[nodiscard]] Window const* window() const;

private:
	AudioSys* audio_sys_;
	InputSys* input_sys_;
	ResourceSys* resource_sys_;
	Director* director_;
	Window* window_;
};

}