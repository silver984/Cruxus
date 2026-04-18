#pragma once
#include <crx/config.hh>

namespace crx {
class AudioManager;
class DebugManager;
class Game;
class InputManager;
class ResourceManager;
class Director;
class Window;

struct CRX_DLL context final {
	friend class Game;
private:
	context();
	explicit context(
		AudioManager* audio,
		DebugManager* debug,
		InputManager* input,
		ResourceManager* resource,
		Director* director,
		Window* window
	);
public:
	[[nodiscard]] Director* const& director();
	[[nodiscard]] Window* const& window();
private:
	AudioManager* audio_;
	DebugManager* debug_;
	InputManager* input_;
	ResourceManager* resource_;
	Director* director_;
	Window* window_;
};
}