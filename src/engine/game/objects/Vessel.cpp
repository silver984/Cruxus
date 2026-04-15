#include <slv/engine/game/objects/Vessel.hpp>
#include <slv/engine/game/managers/WindowManager.hpp>
#include <slv/engine/math.hpp>
#include <algorithm>
#include <cmath>

namespace slv {

Vessel::Vessel() :
	color(255, 255, 255),
	pos(0.f, 0.f),
	anchor(0.5f, 0.5f),
	scale(1.f, 1.f),
	skew(0.f, 0.f),
	rotation(0.f),
	alpha(1.f),
	time_scale(1.f),
	is_visible(true),
	is_active(true),
	bounds_(0.f, 0.f),
	local_transform_(mat3::identity()),
	world_transform_(mat3::identity()),
	world_alpha_(1.f),
	last_rotation_(-1.f),
	last_alpha_(-1.f),
	last_bounds_(-1.f, -1.f),
	last_pos_(-1.f, -1.f),
	last_anchor_(-1.f, -1.f),
	last_scale_(-1.f, -1.f),
	last_skew_(-1.f, -1.f),
	is_dirty_(false),
	is_initialized_(false)
{}

Vessel::~Vessel() = default;

void Vessel::add(sptr<Vessel> vessel) {
	if (!vessel) {
		return;
	}

	auto self = shared_from_this();

	if (vessel == self) {
		// prevent self-parenting
		return;
	}

	if (vessel->has_ancestor(self)) {
		// prevent hierarchy cycle
		return;
	}

	if (std::find(children_.begin(), children_.end(), vessel) != children_.end()) {
		// prevent duplicates
		return;
	}

	if (auto old_parent = vessel->parent().lock()) {
		// remove from old parent
		old_parent->remove(vessel);
	}

	vessel->parent_ = weak_from_this();
	children_.push_back(vessel);
}

void Vessel::remove(sptr<Vessel> vessel) {
	if (!vessel) {
		return;
	}

	auto self = shared_from_this();

	if (vessel == self) {
		// prevent self-remove
		return;
	}

	auto it = std::find(children_.begin(), children_.end(), vessel);
	if (it == children_.end()) {
		return;
	}

	vessel->parent_.reset();
	children_.erase(it);
}

void Vessel::destroy() {
	if (!is_initialized_) {
		return;
	}

	is_initialized_ = false;

	if (auto parent = parent_.lock()) {
		parent->remove(shared_from_this());
	}

	// recursively destroy children
	while (!children_.empty()) {
		auto child = children_.back();
		children_.pop_back();
		if (child) {
			child->parent_.reset();
			child->destroy();
		}
	}
}

size_t Vessel::count() const {
	size_t c = children_.size();

	for (auto const& child : children_) {
		if (!child) {
			continue;
		}
		
		c += child->count();
	}

	return c;
}

float Vessel::world_rotation() const {
	return std::atan2(world_transform_.m[1][0], world_transform_.m[0][0]);
}

vec2<float> Vessel::world_position() const {
	return world_transform_.translation();
}

vec2<float> Vessel::world_scale() const {
	float sx = std::sqrt(
		world_transform_.m[0][0] * world_transform_.m[0][0] +
		world_transform_.m[0][1] * world_transform_.m[0][1]
	);

	float sy = std::sqrt(
		world_transform_.m[1][0] * world_transform_.m[1][0] +
		world_transform_.m[1][1] * world_transform_.m[1][1]
	);

	return vec2<float>(sx, sy);
}

size<float> Vessel::world_size() const {
	return size<float>(
		bounds_.width * world_scale().x,
		bounds_.height * world_scale().y
	);
}

float Vessel::world_alpha() const {
	return world_alpha_;
}

size<float> Vessel::bounds() const {
	return bounds_;
}

wptr<Vessel> Vessel::parent() const {
	return parent_;
}

void Vessel::set_name(std::string_view name) {
	name_ = std::string(name);
}

std::string_view Vessel::name() const {
	return name_;
}

std::string_view Vessel::type() const {
	static constexpr std::string_view TYPE = "Vessel";
	return TYPE;
}

// protected
bool Vessel::init(context const& ctx) {
	return true;
}

// protected
void Vessel::update(context const& ctx, float dt) {}

// protected
void Vessel::draw(context const& ctx) const {}

// protected
mat3 Vessel::world_transform() const {
	return world_transform_;
}

// private
bool Vessel::base_init(context const& ctx) {
	if (is_initialized_) {
		return true;
	}

	if (!init(ctx)) {
		return false;
	}

	is_initialized_ = true;
	return true;
}

// private
void Vessel::base_update(context const& ctx, float dt) {
	if (!is_initialized_ || !is_active) {
		return;
	}

	alpha = std::clamp(alpha, 0.f, 1.f);
	bool window_was_resized = ctx.window ? ctx.window->was_resized() : false;

	if (
		window_was_resized ||
		last_pos_ != pos ||
		last_alpha_ != alpha ||
		last_anchor_ != anchor ||
		last_rotation_ != rotation ||
		last_scale_ != scale ||
		last_bounds_ != bounds_ ||
		last_skew_ != skew
	) {
		last_pos_ = pos;
		last_alpha_ = alpha;
		last_anchor_ = anchor;
		last_rotation_ = rotation;
		last_scale_ = scale;
		last_bounds_ = bounds_;
		last_skew_ = skew;
		mark_dirty();
	}

	if (is_dirty_) {
		auto anchor_offset = vec2<float>(
			anchor.x * bounds_.width,
			anchor.y * bounds_.height
		);
		
		auto skew_rad = vec2<float>(
			slv::math::deg2rad(skew.x),
			slv::math::deg2rad(skew.y)
		);
		
		float rotation_rad = slv::math::deg2rad(rotation);

		mat3 T = mat3::translation(pos);
		mat3 R = mat3::rotation(rotation_rad);
		mat3 S = mat3::scale(scale);
		mat3 K = mat3::skew(skew_rad);
		mat3 A = mat3::translation(-anchor_offset);

		local_transform_ = T * R * S * K * A;

		if (auto p = parent_.lock()) {
			world_transform_ = p->world_transform_ * local_transform_;
			world_alpha_ = std::clamp(alpha * p->world_alpha_, 0.f, 1.f);
		} else {
			float ui_scale = ctx.window ? ctx.window->ui_scale() : 1.f;
			mat3 UI = mat3::scale(vec2<float>(ui_scale, ui_scale));
			world_transform_ = UI * local_transform_;
			world_alpha_ = alpha;
		}

		is_dirty_ = false;
	}

	float world_dt = dt * time_scale;
	update(ctx, world_dt);

	for (auto const& vessel : children_) {
		if (!vessel) {
			continue;
		}

		vessel->base_update(ctx, world_dt);
	}
}

// private
void Vessel::base_draw(context const& ctx) const {
	if (
		!is_initialized_ ||
		!is_visible ||
		world_alpha_ == 0.f
	) {
		return;
	}

	draw(ctx);

	for (auto const& v : children_) {
		if (!v) {
			continue;
		}

		v->base_draw(ctx);
	}
}

// private
bool Vessel::has_ancestor(sptr<Vessel> vessel) const {
	auto p = parent().lock();

	while (p) {
		if (p == vessel) {
			return true;
		}

		p = p->parent().lock();
	}

	return false;
}

// private
void Vessel::mark_dirty() {
	if (!is_dirty_) {
		is_dirty_ = true;

		for (auto& child : children_) {
			if (child) {
				child->mark_dirty();
			}
		}
	}
}

} // namespace slv