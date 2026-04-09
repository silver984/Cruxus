#pragma once
#include <slv/core/dll.hpp>
#include <slv/types/primitives.hpp>
#include <slv/types/colors.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/game_context.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <utility>
#include <cstdint>

namespace slv {

class SceneManager; // forward declare
class SLV_DLL Vessel : public std::enable_shared_from_this<Vessel> {
	friend class SceneManager;

public:
	Vessel();
	virtual ~Vessel();

	template<typename Derived, typename... Args>
	static inline sptr<Derived> create(game_context const& ctx, Args&&... args) {
		static_assert(std::is_base_of_v<Vessel, Derived>);

		sptr<Derived> ptr = shared<Derived>(std::forward<Args>(args)...);

		if (!ptr->base_init(ctx)) {
			ptr.reset();
			return nullptr;
		}

		return ptr;
	}

	void add(sptr<Vessel> vessel);
	void remove(sptr<Vessel> vessel);
	void destroy();
	size_t count() const;
	size_t count_active() const;
	size_t count_visible() const;
	float world_rotation() const;
	vec2<float> world_position() const;
	vec2<float> world_scale() const;
	size<float> world_size() const;
	float world_alpha() const;
	size<float> content_size() const;
	wptr<Vessel> parent() const;
	void set_name(std::string_view name);
	std::string_view name() const;
	virtual std::string_view type() const;

	rgb color;
	vec2<float> pos;
	vec2<float> anchor;
	vec2<float> scale;
	vec2<float> skew; // degrees
	float rotation; // degrees
	float alpha; // 0 - 1
	float time_scale;
	bool is_visible;
	bool is_active;

protected:
	virtual bool init(game_context const& ctx);
	virtual void update(float dt, game_context const& ctx);
	virtual void draw(game_context const& ctx) const;
	mat3 world_transform() const;

	size<float> content_size_;

private:
	bool base_init(game_context const& ctx);
	void base_update(float dt, game_context const& ctx);
	void base_draw(game_context const& ctx) const;
	bool has_ancestor(sptr<Vessel> vessel) const;
	void clean_children();
	void mark_dirty();

	wptr<Vessel> parent_;
	std::vector<sptr<Vessel>> children_;
	std::string name_;
	mat3 local_transform_;
	mat3 world_transform_;
	float world_alpha_;
	float last_rotation_;
	float last_alpha_;
	size<float> last_content_size_;
	vec2<float> last_pos_;
	vec2<float> last_anchor_;
	vec2<float> last_scale_;
	vec2<float> last_skew_;
	bool is_dirty_;
	bool is_initialized_;
};

}