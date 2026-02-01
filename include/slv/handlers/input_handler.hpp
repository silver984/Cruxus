#pragma once

#include <slv/core/types.hpp>
#include <vector>

namespace slv
{
	class InputHandler
	{
	private:
		InputHandler() = default;
		~InputHandler() = default;

	public:
		static inline InputHandler& get()
		{
			static InputHandler instance;
			return instance;
		}

		InputHandler(const InputHandler&) = delete;
		InputHandler(InputHandler&&) = delete;
		InputHandler& operator = (const InputHandler&) = delete;
		InputHandler& operator = (InputHandler&&) = delete;

		void update();
		const slv::key& get_current_down_key() const;
		const std::vector<slv::key>& get_down_keys() const;
		bool is_key_down(const slv::key& key) const;

	private:
		slv::key m_current_down_key = slv::key::NULL_KEY;
		std::vector<slv::key> m_down_keys;
	};
}