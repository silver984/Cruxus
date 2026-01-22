#pragma once

#include <slv/types.hpp>
#include <slv/scene.hpp>
#include <string>
#include <string_view>
#include <memory>

namespace slv
{
	class Game
	{
	public:
		Game() = default;
		~Game();

		bool init(std::string_view window_title, const slv::size_uint& window_size, unsigned int window_fps, bool has_vsync = true, bool is_resizable = true, bool start_fullscreen = false);
		void run();
		void change_scene(std::unique_ptr<Scene> new_scene);

	private:
		static constexpr const char* m_class_name = "Game";
		std::unique_ptr<Scene> m_current_scene = nullptr;
		std::unique_ptr<Scene> m_pending_scene = nullptr;
		bool m_is_init = false;
	};
}