#include <crx/engine/main/InputSys.hh>
#include <crx/engine/debug/log.hh>
#include <raylib.h>

namespace crx {

// private
InputSys::InputSys() :
	cur_key_pressed_(keys::none)
{}

// private
InputSys::~InputSys() = default;

bool InputSys::is_key_down(keys key) const {
	using enum key_down_state;
	return down_keys_[current].test((size_t)key);
}

bool InputSys::is_key_pressed(keys key) const {
	using enum key_down_state;

	size_t idx = (size_t)key;
	return down_keys_[current].test(idx) && !down_keys_[previous].test(idx);
}

bool InputSys::is_key_released(keys key) const {
	using enum key_down_state;

	size_t idx = (size_t)key;
	return !down_keys_[current].test(idx) && down_keys_[previous].test(idx);
}

void InputSys::invoke_key(keys key) {
	using enum key_down_state;
	down_keys_[current].set((size_t)key);
}

keys InputSys::cur_key_pressed() const {
	return cur_key_pressed_;
}

// private
void InputSys::update(float dt) {
	using enum key_down_state;
	
	auto& cur_down_keys = down_keys_[current];
	down_keys_[previous] = cur_down_keys;
	cur_down_keys.reset();
	cur_key_pressed_ = keys::none;

	for (keys key : all_keys) {
		if (IsKeyDown((int)key)) {
			cur_down_keys.set((size_t)key);
		}

		if (IsKeyPressed((int)key)) {
			cur_key_pressed_ = key;
		}
	}
}

}