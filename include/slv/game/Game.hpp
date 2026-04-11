#pragma once
#include <slv/internal/config.hpp>
#include <slv/game/managers/WindowManager.hpp>
#include <slv/game/managers/CrashManager.hpp>
#include <slv/game/managers/AudioManager.hpp>
#include <slv/game/managers/InputManager.hpp>
#include <slv/game/managers/SceneManager.hpp>
#include <slv/game/managers/ResourceManager.hpp>
#include <slv/types/context.hpp>
#include <string_view>

namespace slv {

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
	[[nodiscard]] context ctx();

private:
	WindowManager window_;
	CrashManager crash_;
	AudioManager audio_;
	InputManager input_;
	SceneManager scene_;
	ResourceManager resource_;
	bool is_initialized_;
};

}