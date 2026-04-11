#pragma once

namespace slv {

class WindowManager; // forward declare
class SceneManager; // forward declare
class InputManager; // forward declare
class AudioManager; // forward declare
class ResourceManager; // forward declare

struct context final {
	WindowManager* window;
	SceneManager* scene;
	InputManager* input;
	AudioManager* audio;
	ResourceManager* resource;
};

}