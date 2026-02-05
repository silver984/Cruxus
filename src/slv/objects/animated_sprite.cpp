#include <slv/objects/animated_sprite.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/core/console_log.hpp>
#include <slv/handlers/resource_handler.hpp>

namespace slv
{
	// protected
	bool AnimatedSprite::init()
	{
		m_texture = slv::ResourceHandler::get().load_texture(m_texture_file_path);
		m_atlas_data = slv::ResourceHandler::get().load_atlas_data(m_data_file_path);

		if (!m_texture || !m_atlas_data)
		{
			return false;
		}

		init_size();

		return true;
	}

	// protected
	void AnimatedSprite::update(float dt)
	{
		if (m_antialiasing_check != is_antialiasing && m_texture)
		{
			slv::raylib::set_texture_antialiasing(*m_texture, is_antialiasing);
			m_antialiasing_check = is_antialiasing;
		}

		const auto& current_anim = m_atlas_data->frames[m_current_anim];

		m_frame_elapsed += dt;
		float target_dt = 1.0F / m_fps;
		while (m_frame_elapsed >= target_dt)
		{
			if (m_is_looping)
			{
				m_current_frame_index = (m_current_frame_index + 1) % current_anim.size();
			}
			else
			{
				if (m_current_frame_index < current_anim.size())
				{
					m_current_frame_index++;
				}
			}

			m_frame_elapsed -= target_dt;
		}

		const auto& m_current_frame = current_anim[m_current_frame_index];

		m_source = slv::rect<float>(static_cast<float>(m_current_frame.pos_on_sheet.x),
									static_cast<float>(m_current_frame.pos_on_sheet.y),
									static_cast<float>(m_current_frame.size_on_sheet.width),
									static_cast<float>(m_current_frame.size_on_sheet.height));

		m_dest = slv::rect<float>(this->world_pos_.x - (m_current_frame.offsets.x * this->world_scale_.x),
								  this->world_pos_.y - (m_current_frame.offsets.y * this->world_scale_.y),
								  m_source.width * this->world_scale_.x,
								  m_source.height * this->world_scale_.y);
	}

	// protected
	void AnimatedSprite::draw() const
	{
		if (m_texture)
		{
			slv::raylib::draw_texture(*m_texture, m_source, m_dest, this->anchor, this->world_rotation_, this->world_alpha_, this->color);
		}
	}

	// private
	void AnimatedSprite::init_size()
	{
		slv::size<float> compounded_size;
		size_t total = 0;

		for (const auto& [name, frames] : m_atlas_data->frames)
		{
			for (const auto& f : frames)
			{
				compounded_size.width += f.size_on_sheet.width;
				compounded_size.height += f.size_on_sheet.height;
				total++;
			}
		}

		size_ = compounded_size / total;
	}

	// private
	bool AnimatedSprite::is_anim_found(const std::string& name) const
	{
		if (!m_atlas_data->frames.contains(name))
		{
			slv::console_log(slv::LOG_WARNING, get_type().c_str(), "Animation \"{}\" not found", name);
			return false;
		}

		return true;
	}

	// private
	bool AnimatedSprite::is_alias_found(const std::string& alias) const
	{
		if (!m_aliases.contains(alias))
		{
			slv::console_log(slv::LOG_WARNING, get_type().c_str(), "Alias \"{}\" not found", alias);
			return false;
		}

		return true;
	}

	void AnimatedSprite::add_anim_alias(const std::string& alias, const std::string& name)
	{
		if (!is_anim_found(name))
		{
			return;
		}

		m_aliases.emplace(alias, name);
	}

	void AnimatedSprite::remove_anim_alias(const std::string& alias)
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
		m_fps = fps;
		m_is_looping = is_looping;
	}
}