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

	const WindowManager* const window;
	const SceneManager* const scene;
	const InputManager* const input;
	const AudioManager* const audio;
	const ResourceManager* const resource;
};

}