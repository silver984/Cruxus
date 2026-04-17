#pragma once
#include <crx/config.hh>
#include <crx/engine/Game/objects/Vessel.hpp>
#include <crx/types/context.hpp>
#include <crx/types/pointers.hpp>

namespace crx {

class Game; // forward declare
class CRX_DLL Harbor final {
	friend class Game;

private:
	Harbor();
	~Harbor();

public:
	Harbor(const Harbor&) = delete;
	Harbor(Harbor&&) = delete;
	Harbor& operator=(const Harbor&) = delete;
	Harbor& operator=(Harbor&&) = delete;

	void dock(sptr<Vessel>&& new_scene);
	void destroy_current_scene();
	[[nodiscard]] wptr<Vessel> current_scene();

private:
	void update(context const& ctx, float dt);
	void draw(context const& ctx);
	void safely_destroy_scene();

	sptr<Vessel> current_scene_;
	sptr<Vessel> pending_scene_;
	bool is_destroying_current_scene_;
};

}