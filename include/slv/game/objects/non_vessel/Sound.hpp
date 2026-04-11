#pragma once

#include <slv/core/types/pointers.hpp>
#include <slv/core/types/audio.hpp>
#include <slv/core/types/game_context.hpp>
#include <string>

namespace slv
{
	class Sound
	{
	public:
		Sound() = default;
		~Sound() = default;
		
		bool create(const slv::game_context& ctx, const std::string& file_path);
		void play(const slv::game_context& ctx);

	private:
		slv::sptr<slv::pcm_data> pcm = nullptr;
	};
}