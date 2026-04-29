#pragma once
#include <cruxus/config.hh>

namespace crx {

class AudioSys;
class InputSys;
class ResourceSys;
class Director;
class Window;
class Game;

class CRX_DLL Context final {
	friend class Game;
private:
	Context();
	explicit Context(
		AudioSys* a,
		InputSys* i,
		ResourceSys* r,
		Director* d,
		Window* w
	);

public:
	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator =(Context const&) = delete;
	Context& operator =(Context&&) = delete;

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