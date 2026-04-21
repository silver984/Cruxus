#pragma once
#include <crx/config.hh>
#include <crx/engine/main/Window.hh>
#include <crx/engine/main/AudioSys.hh>
// #include <crx/engine/main/CrashManager.hh>
#include <crx/engine/main/InputSys.hh>
#include <crx/engine/main/ResourceSys.hh>
#include <crx/engine/main/Director.hh>
#include <crx/engine/main/context.hh>
#include <crx/engine/math/size.hh>
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
	[[nodiscard]] context ctx();

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