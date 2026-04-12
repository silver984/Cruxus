#include <slv/objects/non_vessel/Sound.hpp>
#include <slv/game/managers/ResourceManager.hpp>
#include <slv/game/managers/AudioManager.hpp>

namespace slv
{
	bool Sound::create(const slv::game_context& ctx, const std::string& file_path)
	{
		auto resource = ctx.resource_manager;

		if (!resource)
		{
			return false;
		}
		
		pcm = resource->load_pcm_data(file_path);
		if (!pcm)
		{
			return false;
		}

		return true;
	}

	void Sound::play(const slv::game_context& ctx)
	{
		if (auto audio_manager = ctx.audio_manager)
		{
			if (pcm)
			{
				audio_manager->push_pcm_data(pcm);
			}
		}
	}
}