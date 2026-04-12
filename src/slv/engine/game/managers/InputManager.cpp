#include <slv/engine/game/managers/InputManager.hpp>
#include <slv/engine/log.hpp>
#include <raylib.h>

namespace slv {

InputManager::InputManager() :
	cur_key_pressed_(key::NULL_KEY)
{}

InputManager::~InputManager() = default;

bool InputManager::is_key_down(key key_val) const {
	return down_keys_[key_down_state::CURRENT].test(static_cast<size_t>(key_val));
}

bool InputManager::is_key_pressed(key key_val) const {
	size_t idx = static_cast<size_t>(key_val);

	return
		down_keys_[key_down_state::CURRENT].test(idx) &&
		!down_keys_[key_down_state::PREVIOUS].test(idx);
}

bool InputManager::is_key_released(key key_val) const {
	size_t idx = static_cast<size_t>(key_val);

	return
		!down_keys_[key_down_state::CURRENT].test(idx) &&
		down_keys_[key_down_state::PREVIOUS].test(idx);
}

void InputManager::invoke_key(key key_val) {
	down_keys_[key_down_state::CURRENT].set(static_cast<size_t>(key_val));
}

key InputManager::cur_key_pressed() const {
	return cur_key_pressed_;
}

// private
void InputManager::update(float dt) {
	auto& cur_down_keys = down_keys_[key_down_state::CURRENT];
	down_keys_[key_down_state::PREVIOUS] = cur_down_keys;
	cur_down_keys.reset();
	cur_key_pressed_ = key::NULL_KEY;

	for (key key_val : all_keys) {
		if (IsKeyDown(static_cast<int>(key_val))) {
			cur_down_keys.set(static_cast<size_t>(key_val));
		}

		if (IsKeyPressed(static_cast<int>(key_val))) {
			cur_key_pressed_ = key_val;
		}
	}
}

}