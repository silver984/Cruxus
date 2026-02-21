#pragma once

#include <slv/game/managers/SceneManager.hpp>
#include <slv/core/console/log.hpp>

namespace slv
{
	void SceneManager::update(float dt, const slv::game_context& ctx)
	{
		if (m_pending_scene)
		{
			m_current_scene = std::move(m_pending_scene);
		}

		if (m_destroying_current_scene)
		{
			safely_destroy_scene();
			m_destroying_current_scene = false;
		}

		if (m_current_scene)
		{
			m_current_scene->base_update(dt, ctx);
		}
	}

	void SceneManager::draw(const slv::game_context& ctx)
	{
		if (m_current_scene)
		{
			m_current_scene->base_draw(ctx);
		}
	}

	void SceneManager::safely_destroy_scene()
	{
		if (m_current_scene)
		{
			m_current_scene->destroy();
			m_current_scene.reset();
		}
	}

	void SceneManager::change_scene(slv::sptr<slv::Vessel>&& new_scene)
	{
		if (!new_scene)
		{
			slv::log::error(M_NAME, "The scene the game tried to change into is nullptr");
			return;
		}

		m_pending_scene = std::move(new_scene);
	}

	void SceneManager::destroy_current_scene()
	{
		m_destroying_current_scene = true;
	}
}