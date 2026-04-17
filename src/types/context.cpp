#include <slv/types/context.hpp>

namespace crx {

context::context(
	AudioManager* audio_manager,
	DebugManager* debug_manager,
	InputManager* input_manager,
	ResourceManager* resource_manager,
	Harbor* scene_manager,
	Window* window_manager
) :
	audio(audio_manager),
	debug(debug_manager),
	input(input_manager),
	resource(resource_manager),
	scene(scene_manager),
	Window(window_manager)
{}

}