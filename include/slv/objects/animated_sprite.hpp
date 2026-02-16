#pragma once

#include <slv/vessel.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/types/primitives.hpp>
#include <slv/core/types/texture.hpp>
#include <slv/core/types/atlas.hpp>
#include <string>
#include <unordered_map>
#include <memory>

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

		static s_ptr<AnimatedSprite> create(const std::string& texture_file_path, const std::string& data_file_path)
		{
			s_ptr<AnimatedSprite> animated_sprite = shared<AnimatedSprite>(texture_file_path, data_file_path);

			if (!animated_sprite->base_init())
			{
				animated_sprite.reset();
				return nullptr;
			}

			return animated_sprite;
		}

		void add_alias(const std::string& alias, const std::string& name);
		void remove_alias(const std::string& alias);
		void play_alias(const std::string& alias, float fps, bool is_looping = true);
		void play_anim(const std::string& name, float fps, bool is_looping = true);
		void set_anim_offsets(const std::string& name, const slv::vec_2<float>& offsets);
		void set_alias_offsets(const std::string& alias, const slv::vec_2<float>& offsets);

		inline std::string get_type() const override
		{
			return "AnimatedSprite";
		}

		inline std::string get_current_anim()
		{
			return m_current_anim;
		}

		/*
		std::string get_current_anim_alias();
		float get_current_anim_fps();
		bool is_current_anim_looping();
		bool is_current_anim_playing();
		*/

		bool is_antialiasing = true;

	protected:
		bool init() override;
		void update(float dt) override;
		void draw() const override;

	private:
		void init_size();
		bool is_anim_found(const std::string& name) const;
		bool is_alias_found(const std::string& alias) const;

		bool m_antialiasing_check = false;
		float m_fps = 0.0F;
		float m_frame_elapsed = 0.0F;
		bool m_is_looping = false;
		size_t m_current_frame_index = 0;
		std::string m_texture_file_path;
		std::string m_data_file_path;
		slv::rect<float> m_source{};
		slv::rect<float> m_dest{};
		s_ptr<slv::texture> m_texture = nullptr;
		s_ptr<slv::atlas_data> m_atlas_data = nullptr;
		std::unordered_map<std::string, std::string> m_aliases;
		std::unordered_map<std::string, slv::vec_2<float>> m_offsets;
		std::string m_current_anim;
	};
}