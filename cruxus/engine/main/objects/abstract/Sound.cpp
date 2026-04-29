#include <cruxus/engine/Game/objects/abstract/Sound.hh>
#include <cruxus/engine/main/backend/ResourceSys.hh>
#include <cruxus/engine/main/backend/AudioSys.hh>
#include <algorithm>

namespace crx {

// private
Sound::Sound(sptr<std::vector<float>> pcm) :
	volume_(1.f),
	pcm_(pcm)
{}

Sound::Sound() :
	volume_(1.f),
	pcm_(nullptr)
{}

Sound::~Sound() = default;

Sound Sound::create(Context const& ctx, std::string_view file_path) {
	auto& resource = ctx.resource;

	if (!resource) {
		return {};
	}

	return Sound(resource->load_pcm_data(file_path));
}

void Sound::play(Context const& ctx) const {
	if (auto& audio = ctx.audio) {
		audio->push_audio(pcm_, volume_);
	}
}

void Sound::set_volume(float val) {
	volume_ = std::clamp(val, 0.f, 2.f);
}

float Sound::volume() const {
	return volume_;
}

}