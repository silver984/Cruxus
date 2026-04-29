#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/util/pointers.hh>
#include <string>
#include <vector>

namespace crx {

class Game;
class Window;
class CRX_DLL AudioSys final {
	friend class Game;
private:
	AudioSys();
	~AudioSys();
public:
	AudioSys(AudioSys const&) = delete;
	AudioSys(AudioSys&&) = delete;
	AudioSys& operator =(AudioSys const&) = delete;
	AudioSys& operator =(AudioSys&&) = delete;

	void push_audio(sptr<std::vector<float>> pcm, float volume) const;
private:
	bool init(Window* Window);
	void shutdown() const;

	struct impl;
	uptr<impl> impl_;
};

}
