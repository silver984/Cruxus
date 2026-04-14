#include <slv/engine/game/objects/abstract/Sound.hpp>
#include <slv/engine/game/managers/ResourceManager.hpp>
#include <slv/engine/game/managers/AudioManager.hpp>

namespace slv {

// private
Sound::Sound(sptr<std::vector<float>> pcm) :
	volume(1.f),
	pcm_(pcm)
{}

Sound::Sound() :
	volume(1.f),
	pcm_(nullptr)
{}

Sound::~Sound() = default;

Sound Sound::create(context const& ctx, std::string_view file_path) {
	auto& resource = ctx.resource;

	if (!resource) {
		return {};
	}

	return Sound(resource->load_pcm_data(file_path));
}

void Sound::play(context const& ctx) const {
	if (auto& audio = ctx.audio) {
		audio->push_audio(pcm_, volume);
	}
}

}