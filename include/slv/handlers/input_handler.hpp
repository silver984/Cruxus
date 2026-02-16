#pragma once

#ifndef SLV_INPUT_HND
#define SLV_INPUT_HND slv::InputHandler::get()
#endif

#include <slv/core/types/keys.hpp>
#include <slv/core/types/pointers.hpp>
#include <bitset>
#include <vector>
#include <cstdint>

namespace slv
{
	class Game; // forward declare

	class InputHandler
	{
		friend class slv::Game;

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

		bool is_key_down(slv::key key) const;
		bool is_key_pressed(slv::key key) const;
		bool is_key_released(slv::key key) const;
		void invoke_key(slv::key key);
		void add_bind(const slv::sptr<slv::action_bind>& bind);
		bool is_bind_down(const std::string& name) const;
		bool is_bind_pressed(const std::string& name) const;
		bool is_bind_released(const std::string& name) const;
		void invoke_bind(const std::string& name);

	private:
		void update();
		slv::sptr<slv::action_bind> get_bind(const std::string& name) const;

		static constexpr inline const char* M_CLASS_NAME = "InputHandler";
		static constexpr inline size_t M_KEY_COUNT = 348;
		std::bitset<M_KEY_COUNT> m_down_keys;
		std::bitset<M_KEY_COUNT> m_prev_down_keys;
		std::vector<slv::sptr<slv::action_bind>> m_binds;
	};
}