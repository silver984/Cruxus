#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/audio.hpp>
#include <slv/types/pointers.hpp>
#include <string>
#include <vector>

namespace slv {

class Game; // forward declare
class WindowManager; // forward declare
class SLV_DLL AudioManager final {
	friend class Game;

private:
	AudioManager();
	~AudioManager();

public:
	AudioManager(AudioManager const&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	void push_pcm_data(sptr<pcm_data> pcm) const;

private:
	bool init(WindowManager* window);
	void uninit() const;

	struct impl;
	uptr<impl> impl_;
};

}
