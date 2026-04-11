#pragma once

namespace slv {

class WindowManager; // forward declare
class SceneManager; // forward declare
class InputManager; // forward declare
class AudioManager; // forward declare
class ResourceManager; // forward declare

struct context final {
	WindowManager* window;
	SceneManager* scene_manager;
	InputManager* input_manager;
	AudioManager* audio_manager;
	ResourceManager* resource_manager;
};

}