#pragma once
#include <slv/config.hh>

namespace crx {

class AudioManager;
class DebugManager;
class InputManager;
class ResourceManager;
class Harbor;
class Window;

struct SLV_DLL context final {
	AudioManager* const audio;
	DebugManager* const debug;
	InputManager* const input;
	ResourceManager* const resource;
	Harbor* const scene;
	Window* const window_ctx;
};

}