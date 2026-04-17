#pragma once
#include <slv/config.hh>
#include <slv/engine/game/managers/WindowManager.hpp>
#include <slv/types/context.hpp>
#include <slv/engine/math/size.hh>
#include <string_view>

namespace slv {

class AudioManager; // forward declare
class CrashManager; // forward declare
class DebugManager; // forward declare
class InputManager; // forward declare
class ResourceManager; // forward declare
class SceneManager; // forward declare

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
	SceneManager scene_;
	WindowManager window_;
	bool is_initialized_;
	bool is_window_minimized_;
};

}