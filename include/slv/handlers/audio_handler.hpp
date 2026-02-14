#pragma once

#define SLV_AUDIO_HND slv::AudioHandler::get()
#include <string>

namespace slv
{
	class Game; // forward declare

	class AudioHandler
	{
		friend class slv::Game;

	private:
		AudioHandler() = default;
		~AudioHandler();

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
