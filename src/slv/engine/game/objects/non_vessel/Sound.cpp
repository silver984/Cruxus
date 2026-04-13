#include <slv/engine/game/objects/non_vessel/Sound.hpp>
#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/engine/game/managers/AudioManager.hpp>

namespace slv {

Sound::Sound() :
	pcm_data_(nullptr)
{}

Sound::~Sound() = default;

bool Sound::create(context const& ctx, std::string_view file_path) {
	auto& resource = ctx.resource;

	if (!resource) {
		return false;
	}

	pcm_data_ = resource->load_pcm_data(file_path);
	if (!pcm_data_) {
		return false;
	}

	return true;
}

void Sound::play(context const& ctx) {
	if (auto& audio_manager = ctx.audio) {
		if (pcm_data_) {
			audio_manager->push_pcm_data(pcm_data_);
		}
	}
}

}