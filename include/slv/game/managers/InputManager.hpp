#pragma once

#include <slv/core/types/keys.hpp>
#include <slv/core/types/pointers.hpp>
#include <bitset>
#include <vector>
#include <cstdint>

namespace slv
{
	class Game; // forward declare

	class InputManager
	{
		friend class slv::Game;

	private:
		InputManager() = default;
		~InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator=(InputManager&&) = delete;

	public:
		bool is_key_down(slv::key key) const;
		bool is_key_pressed(slv::key key) const;
		bool is_key_released(slv::key key) const;
		void invoke_key(slv::key key);
		void add_bind(const slv::sptr<slv::action_bind>& bind);
		bool is_bind_down(const std::string& name) const;
		bool is_bind_pressed(const std::string& name) const;
		bool is_bind_released(const std::string& name) const;
		void invoke_bind(const std::string& name);

		slv::key current_key_pressed() const
		{
			return m_current_key_pressed;
		}

	private:
		void update(float dt);
		slv::sptr<slv::action_bind> get_bind(const std::string& name) const;

		static constexpr inline const char* M_NAME = "InputManager";
		static constexpr inline float M_CLEANUP_INTERVAL = 1.f;
		static constexpr inline size_t M_KEY_COUNT = 348;
		std::bitset<M_KEY_COUNT> m_down_keys;
		std::bitset<M_KEY_COUNT> m_prev_down_keys;
		std::vector<slv::sptr<slv::action_bind>> m_binds;
		slv::key m_current_key_pressed = slv::key::NULL_KEY;
		float m_since_cleanup = 0.f;
	};
}