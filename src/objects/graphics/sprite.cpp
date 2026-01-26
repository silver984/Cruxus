#include <slv/objects/graphics/sprite.hpp>
#include <slv/objects/graphics/backend/rl_wrappers.hpp>
#include <slv/backend/handlers/resource.hpp>

namespace slv
{
	bool Sprite::init()
	{
		m_texture = slv::ResourceHandler::get().load_texture(m_file_path);

		if (!m_texture)
		{
			return false;
		}

		size_ = m_texture ? slv::size{ static_cast<float>(m_texture->width), static_cast<float>(m_texture->height) } : slv::size{};
		m_source = slv::rect{ 0.f, 0.f, size_.width, size_.height };
		update(0.f);

		return true;
	}

	void Sprite::update(float dt)
	{
		if (m_antialiasing_check != is_antialiasing && m_texture)
		{
			slv::raylib::set_texture_antialiasing(*m_texture, is_antialiasing);
			m_antialiasing_check = is_antialiasing;
		}

		m_dest = slv::rect{ world_pos_.x, world_pos_.y, m_source.width * world_scale_.x, m_source.height * world_scale_.y };
	}

	void Sprite::draw() const
	{
		if (m_texture)
		{
			slv::raylib::draw_texture(*m_texture, m_source, m_dest, anchor, world_rotation_, world_alpha_, color);
		}
	}
}