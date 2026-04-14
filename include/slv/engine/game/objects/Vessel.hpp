#pragma once
#include <slv/internal/config.hpp>
#include <slv/types/primitives.hpp>
#include <slv/types/pointers.hpp>
#include <slv/types/context.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace slv {

class SceneManager;
class SLV_DLL Vessel : public std::enable_shared_from_this<Vessel> {
	friend class SceneManager;

public:
	Vessel();
	virtual ~Vessel();

	template<typename derived, typename... va_args>
	requires std::is_base_of_v<Vessel, derived>
	static sptr<derived> create(context const& ctx, va_args&&... args) {
		sptr<derived> ptr = shared<derived>(std::forward<va_args>(args)...);

		if (!ptr->base_init(ctx)) {
			ptr.reset();
			return nullptr;
		}

		return ptr;
	}

	void add(sptr<Vessel> vessel);
	void remove(sptr<Vessel> vessel);
	void destroy();
	[[nodiscard]] size_t count() const;
	[[nodiscard]] float world_rotation() const;
	[[nodiscard]] vec2<float> world_position() const;
	[[nodiscard]] vec2<float> world_scale() const;
	[[nodiscard]] size<float> world_size() const;
	[[nodiscard]] float world_alpha() const;
	[[nodiscard]] size<float> bounds() const;
	[[nodiscard]] wptr<Vessel> parent() const;
	void set_name(std::string_view name);
	[[nodiscard]] std::string_view name() const;
	[[nodiscard]] virtual std::string_view type() const;

	rgb color;
	vec2<float> pos;
	vec2<float> anchor;
	vec2<float> scale;
	vec2<float> skew; // degrees
	float rotation; // degrees
	float alpha;
	float time_scale;
	bool is_visible;
	bool is_active;

protected:
	virtual bool init(context const& ctx);
	virtual void update(context const& ctx, float dt);
	virtual void draw(context const& ctx) const;
	mat3 world_transform() const;

	size<float> bounds_;

private:
	bool base_init(context const& ctx);
	void base_update(context const& ctx, float dt);
	void base_draw(context const& ctx) const;
	bool has_ancestor(sptr<Vessel> vessel) const;
	void mark_dirty();

	wptr<Vessel> parent_;
	std::vector<sptr<Vessel>> children_;
	std::string name_;
	mat3 local_transform_;
	mat3 world_transform_;
	float world_alpha_;
	float last_rotation_;
	float last_alpha_;
	size<float> last_bounds_;
	vec2<float> last_pos_;
	vec2<float> last_anchor_;
	vec2<float> last_scale_;
	vec2<float> last_skew_;
	bool is_dirty_;
	bool is_initialized_;
};

}