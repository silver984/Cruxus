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

		this->dimensions_ = slv::size<float>(static_cast<float>(m_texture->width), static_cast<float>(m_texture->height));
		m_source = slv::rect<float>(0.f, 0.f, this->dimensions_.width, this->dimensions_.height);
		set_antialiasing(true);
		update(0.f, ctx);

		return true;
	}

	// protected
	void Sprite::draw(const slv::game_context& ctx) const
	{
		if (m_texture && m_source.width > 0.f && m_source.height > 0.f)
		{
			slv::raylib::draw_texture(*m_texture, m_source, slv::vec2<float>(0.f, 0.f), this->world_transform(), this->color, this->world_alpha());
		}
	}
}