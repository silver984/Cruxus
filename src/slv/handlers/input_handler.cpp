#include <slv/handlers/input_handler.hpp>
#include <raylib.h>

namespace slv
{
	void InputHandler::update()
	{
		m_down_keys.clear();
		m_current_down_key = slv::key::NULL_KEY;

		for (slv::key key : slv::all_keys)
		{
			if (!IsKeyDown(static_cast<int>(key)))
			{
				continue;
			}

			m_down_keys.push_back(key);

			if (m_current_down_key == slv::key::NULL_KEY)
			{
				m_current_down_key = key;
			}
		}
	}

	const slv::key& InputHandler::get_current_down_key() const
	{
		return m_current_down_key;
	}

	const std::vector<slv::key>& InputHandler::get_down_keys() const
	{
		return m_down_keys;
	}

	bool InputHandler::is_key_down(const slv::key& key) const
	{
		return std::find(m_down_keys.begin(), m_down_keys.end(), key) != m_down_keys.end();
	}
}