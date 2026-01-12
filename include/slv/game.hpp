#pragma once

#include <slv/types.hpp>
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

	private:
		bool m_is_init = false;
	};
}