#pragma once

#include <slv/core/types.hpp>
#include <slv/core/colors.hpp>
#include <slv/scene.hpp>
#include <string>
#include <memory>

namespace slv
{
	class Game
	{
	public:
		Game() = default;
		~Game();

		bool init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps = 60U,
				  bool window_has_vsync = true, bool is_window_resizable = true, bool window_starts_fullscreen = false, bool is_window_borderless = false, bool is_window_transparent = false);
		void run();
		void change_scene(const std::shared_ptr<slv::Scene>& new_scene);

	private:
		static constexpr const char* M_CLASS_NAME = "Game";
		std::shared_ptr<slv::Scene> m_current_scene = nullptr;
		std::shared_ptr<slv::Scene> m_pending_scene = nullptr;
		bool m_is_init = false;
	};
}