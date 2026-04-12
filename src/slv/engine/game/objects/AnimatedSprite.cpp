#include <slv/objects/AnimatedSprite.hpp>
#include <slv/game/managers/ResourceManager.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/console/log.hpp>
#include <algorithm>

namespace slv
{
	// protected
	bool AnimatedSprite::init(const slv::game_context& ctx)
	{
		auto resource = ctx.resource_manager;

		if (!resource)
		{
			return false;
		}

		m_texture = resource->load_texture(m_texture_file_path);
		m_atlas_data = resource->load_atlas_data(m_data_file_path);

		if (!m_texture || !m_atlas_data)
		{
			return false;
		}

		init_size();

		if (!m_atlas_data->frames.empty())
		{
			m_current_anim = m_atlas_data->frames.begin()->first;
		}

		for (const auto& [name, frames] : m_atlas_data->frames)
		{
			m_offsets.insert({ name, slv::vec2<float>(0.f, 0.f) });
		}

		set_antialiasing(true);
		update(0.f, ctx);
		
		return true;
	}

	// protected
	void AnimatedSprite::update(float dt, const slv::game_context& ctx)
	{
		auto it = m_atlas_data->frames.find(m_current_anim);
		if (it == m_atlas_data->frames.end() || it->second.empty())
		{
			return;
		}

		const auto& current_frames = it->second;

		if (fps > 0.f)
		{
			m_frame_elapsed += dt;
			float target_dt = 1.f / fps;

			while (m_frame_elapsed >= target_dt)
			{
				if (m_is_looping)
				{
					m_current_frame_index = (m_current_frame_index + 1) % current_frames.size();
				}
				else
				{
					m_current_frame_index = std::min(m_current_frame_index + 1, current_frames.size() - 1);
				}

				m_frame_elapsed -= target_dt;
			}
		}

		const auto& current_frame = current_frames[m_current_frame_index];

		m_source = slv::rect<float>(static_cast<float>(current_frame.pos_on_sheet.x),
									static_cast<float>(current_frame.pos_on_sheet.y),
									static_cast<float>(current_frame.size_on_sheet.width),
									static_cast<float>(current_frame.size_on_sheet.height));

		auto atlas_offsets = slv::vec2<float>(static_cast<float>(current_frame.offsets.x), static_cast<float>(current_frame.offsets.y));
		m_current_offsets = m_offsets[m_current_anim] - atlas_offsets;
	}

	// protected
	void AnimatedSprite::draw(const slv::game_context& ctx) const
	{
		if (m_texture && m_source.width > 0.f && m_source.height > 0.f)
		{
			slv::raylib::draw_texture(*m_texture, m_source, m_current_offsets, this->world_transform(), this->color, this->world_alpha());
		}
	}

	// private
	void AnimatedSprite::init_size()
	{
		slv::size<float> compounded_size;
		size_t total = 0;

		for (const auto& [name, f] : m_atlas_data->frames)
		{
			for (const auto& frames : f)
			{
				compounded_size.width += frames.size_on_sheet.width;
				compounded_size.height += frames.size_on_sheet.height;
				total++;
			}
		}

		if (total != 0)
		{
			dimensions_ = compounded_size / total;
		}
	}

	// private
	bool AnimatedSprite::is_anim_found(const std::string& name) const
	{
		if (!m_atlas_data->frames.contains(name))
		{
			slv::log::warning(type(), "Animation \"{}\" not found", name);
			return false;
		}

		return true;
	}

	// private
	bool AnimatedSprite::is_alias_found(const std::string& alias) const
	{
		if (!m_aliases.contains(alias))
		{
			slv::log::warning(type(), "Alias \"{}\" not found", alias);
			return false;
		}

		return true;
	}

	void AnimatedSprite::add_alias(const std::string& alias, const std::string& name)
	{
		if (!is_anim_found(name))
		{
			return;
		}

		m_aliases.insert_or_assign(alias, name);
	}

	void AnimatedSprite::remove_alias(const std::string& alias)
	{
		if (is_alias_found(alias))
		{
			m_aliases.erase(alias);
		}
	}

	void AnimatedSprite::play_alias(const std::string& alias, float fps, bool is_looping)
	{
		if (!is_alias_found(alias))
		{
			return;
		}

		play_anim(m_aliases[alias], fps, is_looping);
	}

	void AnimatedSprite::play_anim(const std::string& name, float fps, bool is_looping)
	{
		if (!is_anim_found(name))
		{
			return;
		}

		m_current_anim = name;
		float clamped_fps = std::max(0.f, fps);
		this->fps = clamped_fps == 0.f ? this->fps : clamped_fps;
		m_is_looping = is_looping;
		m_current_frame_index = 0;
	}

	void AnimatedSprite::set_anim_offsets(const std::string& name, const slv::vec2<float>& offsets)
	{
		if (!is_anim_found(name))
		{
			return;
		}

		m_offsets[name] = offsets;
	}

	void AnimatedSprite::set_alias_offsets(const std::string& alias, const slv::vec2<float>& offsets)
	{
		if (!is_alias_found(alias))
		{
			return;
		}

		set_anim_offsets(m_aliases[alias], offsets);
	}

	void AnimatedSprite::set_antialiasing(bool val)
	{
		if (m_texture)
		{
			slv::raylib::set_texture_antialiasing(*m_texture, val);
		}
	}
}