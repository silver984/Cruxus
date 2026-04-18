#pragma once
#include <crx/config.hh>
#include <crx/engine/util/enum_array.hh>
#include <crx/engine/input/keys.hh>
#include <bitset>

namespace crx {

class Game;
class CRX_DLL InputSys final {
	friend class Game;

private:
	InputSys();
	~InputSys();

	enum class key_down_state : int {
		current,
		previous,
		count
	};

public:
	InputSys(InputSys const&) = delete;
	InputSys(InputSys&&) = delete;
	InputSys& operator =(InputSys const&) = delete;
	InputSys& operator =(InputSys&&) = delete;

	[[nodiscard]] bool is_key_down(keys key) const;
	[[nodiscard]] bool is_key_pressed(keys key) const;
	[[nodiscard]] bool is_key_released(keys key) const;
	void invoke_key(keys key);
	[[nodiscard]] keys cur_key_pressed() const;

private:
	void update(float dt);

	enum_array<std::bitset<348>, key_down_state, key_down_state::count> down_keys_;
	keys cur_key_pressed_;
};

}