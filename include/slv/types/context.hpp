#pragma once
#include <slv/config.hpp>

namespace slv {

class AudioManager; // forward declare
class DebugManager; // forward declare
class InputManager; // forward declare
class ResourceManager; // forward declare
class SceneManager; // forward declare
class WindowManager; // forward declare

struct SLV_DLL context final {
	explicit context(
		AudioManager* audio_manager,
		DebugManager* debug_manager,
		InputManager* input_manager,
		ResourceManager* resource_manager,
		SceneManager* scene_manager,
		WindowManager* window_manager
	);

	AudioManager* const audio;
	DebugManager* const debug;
	InputManager* const input;
	ResourceManager* const resource;
	SceneManager* const scene;
	WindowManager* const window;
};

}