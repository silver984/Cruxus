#pragma once
#include <crx/config.hh>

namespace crx {
class AudioSys;
class Game;
class InputSys;
class ResourceSys;
class Director;
class Window;

class CRX_DLL Context final {
	friend class Game;
private:
	Context();
	explicit Context(
		AudioSys* audio_sys,
		InputSys* input_sys,
		ResourceSys* resource_sys,
		Director* director,
		Window* window
	);
	~Context();
public:
	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator =(Context const&) = delete;
	Context& operator =(Context&&) = delete;

	[[nodiscard]] InputSys* const& input_sys();
	[[nodiscard]] AudioSys* const& audio_sys();
	[[nodiscard]] ResourceSys* const& resource_sys();
	[[nodiscard]] Director* const& director();
	[[nodiscard]] Window* const& window();
private:
	AudioSys* audio_sys_;
	InputSys* input_sys_;
	ResourceSys* resource_sys_;
	Director* director_;
	Window* window_;
};
}