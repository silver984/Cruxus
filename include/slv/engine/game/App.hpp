#pragma once
#include <slv/internal/config.hpp>
#include <slv/engine/game/managers/all.hpp>
#include <slv/types/context.hpp>
#include <slv/types/size.hpp>
#include <string_view>

namespace slv {

class SLV_DLL App final {
public:
	App();
	~App();
	App(App const&) = delete;
	App(App&&) = delete;
	App& operator=(App const&) = delete;
	App& operator=(App&&) = delete;

	bool init(
		std::string_view win_title,
		size<int> const& win_size,
		int win_fps,
		window_settings win_settings
	);
	void run();
	[[nodiscard]] context get_ctx();

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