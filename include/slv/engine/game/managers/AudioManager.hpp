#pragma once

#include <slv/core/types/audio.hpp>
#include <slv/core/types/pointers.hpp>
#include <string>
#include <vector>

namespace slv
{
	class Game; // forward declare

	class AudioManager
	{
		friend class slv::Game;

	private:
		AudioManager() = default;
		~AudioManager() = default;
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		AudioManager(AudioManager&&) = delete;
		AudioManager& operator=(AudioManager&&) = delete;
	
	public:
		bool push_pcm_data(const slv::sptr<slv::pcm_data>& pcm) const;

	private:
		static constexpr inline const char* M_NAME = "AudioManager";

		bool init();
		void uninit() const;
	};
}
