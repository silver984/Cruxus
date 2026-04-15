#include <slv/types/context.hpp>

namespace slv {

context::context(
	WindowManager* window_manager,
	SceneManager* scene_manager,
	InputManager* input_manager,
	AudioManager* audio_manager,
	ResourceManager* resource_manager
) :
	window(window_manager),
	scene(scene_manager),
	input(input_manager),
	audio(audio_manager),
	resource(resource_manager)
{}

}