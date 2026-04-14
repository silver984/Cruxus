#pragma once
#include <slv/internal/config.hpp>
#include <slv/engine/game/objects/Vessel.hpp>
#include <slv/types/context.hpp>
#include <slv/types/pointers.hpp>

namespace slv {

class App; // forward declare
class SLV_DLL SceneManager final {
	friend class App;

private:
	SceneManager();
	~SceneManager();

public:
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	void change_scene(sptr<Vessel>&& new_scene);
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