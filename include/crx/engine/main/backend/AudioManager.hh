#pragma once
#include <crx/config.hh>
#include <crx/engine/util/pointers.hh>
#include <string>
#include <vector>

namespace crx {

class Game; // forward declare
class Window; // forward declare
class CRX_DLL AudioManager final {
	friend class Game;

private:
	AudioManager();
	~AudioManager();

public:
	AudioManager(AudioManager const&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	void push_audio(sptr<std::vector<float>> pcm, float volume) const;

private:
	bool init(Window* Window);
	void uninit() const;

	struct impl;
	uptr<impl> impl_;
};

}
