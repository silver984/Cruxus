#pragma once

#ifndef SLV_AUDIO_HND
#define SLV_AUDIO_HND slv::AudioHandler::get()
#endif

#include <string>

namespace slv
{
	class Game; // forward declare

	class AudioHandler
	{
		friend class slv::Game;

	private:
		AudioHandler() = default;
		~AudioHandler()
		{
			uninit();
		}

	public:
		static inline AudioHandler& get()
		{
			static AudioHandler instance;
			return instance;
		}

		AudioHandler(const AudioHandler&) = delete;
		AudioHandler(AudioHandler&&) = delete;
		AudioHandler& operator = (const AudioHandler&) = delete;
		AudioHandler& operator = (AudioHandler&&) = delete;

		bool play_audio(const std::string& file_path) const;

	private:
		static constexpr inline const char* M_CLASS_NAME = "AudioHandler";

		bool init();
		void uninit() const;
	};
}
