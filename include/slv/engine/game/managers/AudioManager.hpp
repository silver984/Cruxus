#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/audio.hpp>
#include <slv/types/pointers.hpp>
#include <string>
#include <vector>

namespace slv {

class App; // forward declare
class SLV_DLL AudioManager final {
	friend class App;

private:
	AudioManager();
	~AudioManager();

public:
	AudioManager(AudioManager const&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	bool push_pcm_data(sptr<pcm_data> const& pcm) const;

private:
	bool init();
	void uninit() const;
};

}
