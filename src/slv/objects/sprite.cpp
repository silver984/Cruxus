#include <slv/objects/sprite.hpp>
#include <slv/core/wrappers/raylib.hpp>
#include <slv/game/managers/ResourceManager.hpp>

namespace slv
{
	void Sprite::set_antialiasing(bool val)
	{
		if (m_texture)
		{
			slv::raylib::set_texture_antialiasing(*m_texture, val);
		}
	}

	// protected
	bool Sprite::init(const slv::game_context& ctx)
	{
		auto resource = ctx.resource_manager;

		if (!resource)
		{
			return false;
		}

		m_texture = resource->load_texture(m_file_path);

		if (!m_texture)
		{
			return false;
		}

		this->size_ = m_texture ? slv::size<float>(static_cast<float>(m_texture->width), static_cast<float>(m_texture->height)) : slv::size<float>(0.f, 0.f);
		m_source = slv::rect<float>(0.f, 0.f, this->size_.width, this->size_.height);
		set_antialiasing(true);
		update(0.f, ctx);

		return true;
	}

	// protected
	void Sprite::update(float dt, const slv::game_context& ctx)
	{
		m_dest = slv::rect<float>(this->world_pos_.x, this->world_pos_.y, m_source.width * this->world_scale_.x, m_source.height * this->world_scale_.y);
	}

	// protected
	void Sprite::draw(const slv::game_context& ctx) const
	{
		if (m_texture &&
			m_source.width > 0.f && m_source.height > 0.f &&
			m_dest.width > 0.f && m_dest.height > 0.f)
		{
			slv::raylib::draw_texture(*m_texture, m_source, m_dest, this->world_anchor_, this->world_rotation_, this->world_alpha_, this->color);
		}
	}
}