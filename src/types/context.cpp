#include <slv/types/context.hpp>

namespace slv {

context::context(
	AudioManager* audio_manager,
	DebugManager* debug_manager,
	InputManager* input_manager,
	ResourceManager* resource_manager,
	SceneManager* scene_manager,
	WindowManager* window_manager
) :
	audio(audio_manager),
	debug(debug_manager),
	input(input_manager),
	resource(resource_manager),
	scene(scene_manager),
	window(window_manager)
{}

}