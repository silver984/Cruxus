#pragma once

namespace slv
{
	class Window; // forward declare
	class SceneManager; // forward declare
	class InputManager; // forward declare
	class AudioManager; // forward declare
	class ResourceManager; // forward declare
	
	struct game_context
	{
		slv::Window* window;
		slv::SceneManager* scene_manager;
		slv::InputManager* input_manager;
		slv::AudioManager* audio_manager;
		slv::ResourceManager* resource_manager;
	};
}