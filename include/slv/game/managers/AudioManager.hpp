#pragma once

#include <string>

namespace slv
{
	class Game; // forward declare

	class AudioManager
	{
		friend class slv::Game;

	private:
		AudioManager() = default;
		~AudioManager()
		{
			uninit();
		}
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		AudioManager(AudioManager&&) = delete;
		AudioManager& operator=(AudioManager&&) = delete;
	
	public:
		bool play_audio(const std::string& file_path) const;

	private:
		static constexpr inline const char* M_NAME = "AudioManager";

		bool init();
		void uninit() const;
	};
}
