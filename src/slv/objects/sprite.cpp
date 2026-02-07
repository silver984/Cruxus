#include <slv/objects/sprite.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/handlers/resource_handler.hpp>

namespace slv
{
	// protected
	bool Sprite::init()
	{
		m_texture = slv::ResourceHandler::get().load_texture(m_file_path);

		if (!m_texture)
		{
			return false;
		}

		this->size_ = m_texture ? slv::size<float>(static_cast<float>(m_texture->width),
												   static_cast<float>(m_texture->height)) : slv::size<float>(0.0F, 0.0F);
		m_source = slv::rect<float>(0.0F, 0.0F, this->size_.width, this->size_.height);
		update(0.0F);

		return true;
	}

	// protected
	void Sprite::update(float dt)
	{
		if (m_antialiasing_check != is_antialiasing && m_texture)
		{
			slv::raylib::set_texture_antialiasing(*m_texture, is_antialiasing);
			m_antialiasing_check = is_antialiasing;
		}

		m_dest = slv::rect<float>(this->world_pos_.x, this->world_pos_.y, m_source.width * this->world_scale_.x, m_source.height * this->world_scale_.y);
	}

	// protected
	void Sprite::draw() const
	{
		if (m_texture &&
			m_source.width > 0.0F && m_source.height > 0.0F &&
			m_dest.width > 0.0F && m_dest.height > 0.0F)
		{
			slv::raylib::draw_texture(*m_texture, m_source, m_dest, this->world_anchor_, this->world_rotation_, this->world_alpha_, this->color);
		}
	}
}