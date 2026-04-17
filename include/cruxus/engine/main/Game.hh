#pragma once
#include <slv/config.hh>
#include <slv/engine/Game/managers/window.hpp>
#include <slv/types/context.hpp>
#include <slv/engine/math/size.hh>
#include <string_view>

namespace crx {

class AudioManager;
class CrashManager;
class DebugManager;
class InputManager;
class ResourceManager;
class Harbor;

class SLV_DLL Game final {
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
		window_settings win_settings
	);
	void run();
	
	[[nodiscard]] context get_ctx();

private:
	AudioManager audio_;
	CrashManager crash_;
	DebugManager debug_;
	InputManager input_;
	ResourceManager resource_;
	Harbor scene_;
	window window_;
	bool is_initialized_;
	bool is_window_minimized_;
};

}