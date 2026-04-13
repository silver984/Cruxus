#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/enum_array.hpp>
#include <slv/types/keys.hpp>
#include <bitset>

namespace slv {

class App; // forward declare
class SLV_DLL InputManager final {
	friend class App;

private:
	InputManager();
	~InputManager();

	enum class key_down_state : int {
		CURRENT,
		PREVIOUS,
		count
	};

public:
	InputManager(InputManager const&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager const&) = delete;
	InputManager& operator=(InputManager&&) = delete;

	[[nodiscard]] bool is_key_down(key key_val) const;
	[[nodiscard]] bool is_key_pressed(key key_val) const;
	[[nodiscard]] bool is_key_released(key key_val) const;
	void invoke_key(key key);
	[[nodiscard]] key cur_key_pressed() const;

private:
	void update(float dt);

	enum_array<std::bitset<348>, key_down_state, key_down_state::count> down_keys_;
	key cur_key_pressed_;
};

}