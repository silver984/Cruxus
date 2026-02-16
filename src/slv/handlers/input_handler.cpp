#include <slv/handlers/input_handler.hpp>
#include <slv/core/console_log.hpp>
#include <raylib.h>

namespace slv
{
	// private
	void InputHandler::update()
	{
		m_prev_down_keys = m_down_keys;
		m_down_keys.reset();

		// clean up binds
		for (auto it = m_binds.begin(); it != m_binds.end(); /**/)
		{
			if (it->use_count() <= 1)
			{
				auto bind_ptr = it->get();
				std::string name = bind_ptr->name;
				int key = static_cast<int>(bind_ptr->key);
				it = m_binds.erase(it);
				slv::console_log(slv::log::trace, M_CLASS_NAME, "Erased bind: \"{}\" | key: {}", name, key);
			}
			else
			{
				++it;
			}
		}

		for (slv::key key : slv::all_keys)
		{
			if (IsKeyDown(static_cast<int>(key)))
			{
				m_down_keys.set(static_cast<size_t>(key));
			}
		}
	}

	bool InputHandler::is_key_down(slv::key key) const
	{
		return m_down_keys.test(static_cast<size_t>(key));
	}

	bool InputHandler::is_key_pressed(slv::key key) const
	{
		size_t idx = static_cast<size_t>(key);
		return m_down_keys.test(idx) && !m_prev_down_keys.test(idx);
	}

	bool InputHandler::is_key_released(slv::key key) const
	{
		size_t idx = static_cast<size_t>(key);
		return !m_down_keys.test(idx) && m_prev_down_keys.test(idx);
	}

	void InputHandler::invoke_key(slv::key key)
	{
		m_down_keys.set(static_cast<size_t>(key));
	}

	void InputHandler::add_bind(const std::shared_ptr<slv::action_bind>& bind)
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
		
		slv::console_log(slv::log::trace, M_CLASS_NAME, "Added bind: \"{}\" | key: {}", bind_ptr->name, static_cast<int>(bind_ptr->key));
	}

	bool InputHandler::is_bind_down(const std::string& name) const
	{
		auto bind = slv::InputHandler::get().get_bind(name);
		if (bind)
		{
			return is_key_down(bind->key);
		}

		return false;
	}

	bool InputHandler::is_bind_pressed(const std::string& name) const
	{
		auto bind = slv::InputHandler::get().get_bind(name);
		if (bind)
		{
			return is_key_pressed(bind->key);
		}

		return false;
	}

	bool InputHandler::is_bind_released(const std::string& name) const
	{
		auto bind = slv::InputHandler::get().get_bind(name);
		if (bind)
		{
			return is_key_released(bind->key);
		}

		return false;
	}

	void InputHandler::invoke_bind(const std::string& name)
	{
		auto bind = slv::InputHandler::get().get_bind(name);
		if (bind)
		{
			invoke_key(bind->key);
		}
	}
}