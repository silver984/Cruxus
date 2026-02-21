#include <slv/game/managers/InputManager.hpp>
#include <slv/core/console/log.hpp>
#include <raylib.h>

namespace slv
{
	// private
	void InputManager::update(float dt)
	{
		m_prev_down_keys = m_down_keys;
		m_down_keys.reset();
		m_current_key_pressed = slv::key::NULL_KEY;

		m_since_cleanup += dt;
		while (m_since_cleanup >= M_CLEANUP_INTERVAL)
		{
			clean_cache();
			m_since_cleanup -= M_CLEANUP_INTERVAL;
		}

		for (slv::key key : slv::all_keys)
		{
			if (IsKeyDown(static_cast<int>(key)))
			{
				m_down_keys.set(static_cast<size_t>(key));
			}

			if (IsKeyPressed(static_cast<int>(key)))
			{
				m_current_key_pressed = key;
			}
		}
	}

	void InputManager::clean_cache()
	{
		for (auto it = m_binds.begin(); it != m_binds.end();)
		{
			if (it->use_count() <= 1)
			{
				auto bind_ptr = it->get();
				std::string name = bind_ptr->name;
				int key = static_cast<int>(bind_ptr->key);
				it = m_binds.erase(it);
				slv::log::trace(M_NAME, "Erased bind: \"{}\" | key: {}", name, key);
			}
			else
			{
				++it;
			}
		}
	}

	// private
	slv::sptr<slv::action_bind> InputManager::get_bind(const std::string& name) const
	{
		for (const auto& bind : m_binds)
		{
			if (bind.get()->name == name)
			{
				return bind;
			}
		}

		return nullptr;
	}

	bool InputManager::is_key_down(slv::key key) const
	{
		return m_down_keys.test(static_cast<size_t>(key));
	}

	bool InputManager::is_key_pressed(slv::key key) const
	{
		size_t idx = static_cast<size_t>(key);
		return m_down_keys.test(idx) && !m_prev_down_keys.test(idx);
	}

	bool InputManager::is_key_released(slv::key key) const
	{
		size_t idx = static_cast<size_t>(key);
		return !m_down_keys.test(idx) && m_prev_down_keys.test(idx);
	}

	void InputManager::invoke_key(slv::key key)
	{
		m_down_keys.set(static_cast<size_t>(key));
	}

	void InputManager::add_bind(const slv::sptr<slv::action_bind>& bind)
	{
		auto bind_ptr = bind.get();

		if (bind_ptr->name.empty())
		{
			return;
		}

		if (bind_ptr->key == slv::key::NULL_KEY)
		{
			return;
		}

		m_binds.emplace_back(bind);
		
		slv::log::trace(M_NAME, "Added bind: \"{}\" | key: {}", bind_ptr->name, static_cast<int>(bind_ptr->key));
	}

	bool InputManager::is_bind_down(const std::string& name) const
	{
		auto bind = get_bind(name);
		if (bind)
		{
			return is_key_down(bind->key);
		}

		return false;
	}

	bool InputManager::is_bind_pressed(const std::string& name) const
	{
		auto bind = get_bind(name);
		if (bind)
		{
			return is_key_pressed(bind->key);
		}

		return false;
	}

	bool InputManager::is_bind_released(const std::string& name) const
	{
		auto bind = get_bind(name);
		if (bind)
		{
			return is_key_released(bind->key);
		}

		return false;
	}

	void InputManager::invoke_bind(const std::string& name)
	{
		auto bind = get_bind(name);
		if (bind)
		{
			invoke_key(bind->key);
		}
	}
}