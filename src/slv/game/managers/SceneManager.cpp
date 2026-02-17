#pragma once

#include <slv/game/managers/SceneManager.hpp>
#include <slv/core/console_log.hpp>

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
			if (m_current_scene)
			{
				m_current_scene->destroy();
				m_current_scene.reset();
			}

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

	void SceneManager::change_scene(slv::sptr<Scene>&& new_scene)
	{
		if (!new_scene)
		{
			slv::console_log(slv::log::ERROR, M_NAME, "The scene the game tried to change into is nullptr");
			return;
		}

		m_pending_scene = std::move(new_scene);
	}

	void SceneManager::destroy_current_scene()
	{
		m_destroying_current_scene = true;
	}
}