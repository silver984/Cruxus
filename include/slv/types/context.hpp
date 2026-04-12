#pragma once
#include <slv/internal/config.hpp>

namespace slv {

class WindowManager; // forward declare
class SceneManager; // forward declare
class InputManager; // forward declare
class AudioManager; // forward declare
class ResourceManager; // forward declare

struct SLV_DLL context final {
	explicit context(
		WindowManager* window_manager,
		SceneManager* scene_manager,
		InputManager* input_manager,
		AudioManager* audio_manager,
		ResourceManager* resource_manager
	);

	WindowManager* const window;
	SceneManager* const scene;
	InputManager* const input;
	AudioManager* const audio;
	ResourceManager* const resource;
};

}