#pragma once
#include <cruxus/config.hh>
#include <cruxus/engine/main/objects/Node.hh>
#include <cruxus/engine/main/Context.hh>
#include <cruxus/engine/util/pointers.hh>

namespace crx {

class Game;
class CRX_DLL Director final {
	friend class Game;
private:
	Director();
	~Director();

public:
	Director(const Director&) = delete;
	Director(Director&&) = delete;
	Director& operator=(const Director&) = delete;
	Director& operator=(Director&&) = delete;

	void switch_scene(sptr<Node>&& new_scene);
	void destroy_current_scene();
	[[nodiscard]] wptr<Node> current_scene();

private:
	void update(Context const& ctx, float dt);
	void draw(Context const& ctx);
	void safely_destroy_scene();

	sptr<Node> current_scene_;
	sptr<Node> pending_scene_;
	bool is_destroying_current_scene_;
};

}