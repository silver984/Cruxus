#pragma once
#include <crx/config.hh>

namespace crx {
class AudioManager;
class DebugManager;
class InputManager;
class ResourceManager;
class Director;
class Window;

struct CRX_DLL context final {
	AudioManager const* const audio;
	DebugManager const* const debug;
	InputManager const* const input;
	ResourceManager const* const resource;
	Director const* const director;
	Window const* const window;
};
}