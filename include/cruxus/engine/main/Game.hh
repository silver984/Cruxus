#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/main/Window.hh>
#include <cruxus/engine/main/AudioSys.hh>
// #include <cruxus/engine/main/CrashManager.hh>
#include <cruxus/engine/main/InputSys.hh>
#include <cruxus/engine/main/ResourceSys.hh>
#include <cruxus/engine/main/Director.hh>
#include <cruxus/engine/main/Context.hh>
#include <cruxus/engine/math/size.hh>
#include <string_view>

namespace crx {

class CRX_DLL Game final {
public:
	Game();
	~Game();
	Game(Game const&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game const&) = delete;
	Game& operator=(Game&&) = delete;

	bool init(
		std::string_view win_title,
		size<int> const& win_size,
		int win_fps,
		Window::config& win_config
	);
	void run();
	[[nodiscard]] Context ctx();

private:
	void shutdown();

	AudioSys audio_sys_;
	// CrashManager crash_;
	InputSys input_sys_;
	ResourceSys resource_sys_;
	Director director_;
	Window window_;
	bool is_initialized_;
};

}