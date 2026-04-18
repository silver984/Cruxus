#pragma once
#include <crx/config.hh>
#include <crx/engine/main/objects/Node.hh>
#include <crx/engine/main/context.hh>
#include <crx/engine/util/pointers.hh>

namespace crx {
class Game; // forward declare
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

	void push_scene(sptr<Node>&& new_scene);
	void pop_scene();
	[[nodiscard]] wptr<Node> current_scene();
private:
	void update(context const& ctx, float dt);
	void draw(context const& ctx);
	void safely_destroy_scene();

	sptr<Node> current_scene_;
	sptr<Node> pending_scene_;
	bool is_destroying_current_scene_;
};
}