#include <crx/engine/main/context.hh>

namespace crx {

context::context(
	AudioSys* audio_manager,
	DebugManager* debug_manager,
	InputSys* input_manager,
	ResourceSys* resource_manager,
	Director* scene_manager,
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