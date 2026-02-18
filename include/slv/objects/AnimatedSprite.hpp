#pragma once

#include <slv/objects/Vessel.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <slv/core/types/atlas.hpp>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace slv
{
	class AnimatedSprite : public slv::Vessel
	{
	public:
		AnimatedSprite(const std::string& texture_file_path, const std::string& data_file_path)
		{
			m_texture_file_path = texture_file_path;
			m_data_file_path = data_file_path;
		}

		void add_alias(const std::string& alias, const std::string& name);
		void remove_alias(const std::string& alias);
		// fps = 0.f (automatic)
		void play_alias(const std::string& alias, float fps = 0.f, bool is_looping = true);
		// fps = 0.f (automatic)
		void play_anim(const std::string& name, float fps = 0.f, bool is_looping = true);
		void set_anim_offsets(const std::string& name, const slv::vec2<float>& offsets);
		void set_alias_offsets(const std::string& alias, const slv::vec2<float>& offsets);
		void set_antialiasing(bool val);

		inline std::string type() const override
		{
			return "AnimatedSprite";
		}

		inline std::string current_anim()
		{
			return m_current_anim;
		}

		/* // to implement later
		std::string get_current_anim_alias();
		float get_current_anim_fps();
		bool is_current_anim_looping();
		bool is_current_anim_playing();
		*/

		float fps = 24.f;

	protected:
		bool init(const slv::game_context& ctx) override;
		void update(float dt, const slv::game_context& ctx) override;
		void draw(const slv::game_context& ctx) const override;

	private:
		void init_size();
		bool is_anim_found(const std::string& name) const;
		bool is_alias_found(const std::string& alias) const;

		bool m_antialiasing_check = false;
		float m_frame_elapsed = 0.f;
		bool m_is_looping = false;
		size_t m_current_frame_index = 0;
		std::string m_texture_file_path;
		std::string m_data_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		slv::sptr<slv::texture> m_texture = nullptr;
		slv::sptr<slv::atlas_data> m_atlas_data = nullptr;
		std::unordered_map<std::string, std::string> m_aliases;
		std::unordered_map<std::string, slv::vec2<float>> m_offsets;
		std::string m_current_anim;
	};
}