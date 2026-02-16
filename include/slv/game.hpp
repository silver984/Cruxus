#pragma once

#include <slv/core/types/pointers.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/scene.hpp>
#include <string>

namespace slv
{
	class Game
	{
	public:
		Game() = default;
		~Game();

		bool init(const std::string& window_title, const slv::size<unsigned int>& window_size, unsigned int fps = 60,
				  bool window_has_vsync = true, bool is_window_resizable = true, bool window_starts_fullscreen = false, bool is_window_borderless = false, bool is_window_transparent = false);
		void run();
		void change_scene(s_ptr<Scene> new_scene);
		void destroy_current_scene();

	private:
		static constexpr const char* M_CLASS_NAME = "Game";
		s_ptr<Scene> m_current_scene = nullptr;
		s_ptr<Scene> m_pending_scene = nullptr;
		bool m_is_init = false;
		bool m_destroying_current_scene = false;
	};
}