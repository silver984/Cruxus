#pragma once

#include <slv/core/types/primitives.hpp>
#include <slv/core/types/colors.hpp>
#include <slv/core/types/pointers.hpp>
#include <slv/core/types/game_context.hpp>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <cstdint>

namespace slv
{
	class SceneManager; // forward declare

	class Vessel : public std::enable_shared_from_this<Vessel>
	{
		friend class slv::SceneManager;

	public:
		Vessel() = default;
		inline virtual ~Vessel() = default;

		template<typename T, typename... args>
		static inline slv::sptr<T> create(const slv::game_context& ctx, args&&... _args)
		{
			static_assert(std::is_base_of_v<Vessel, T>);

			slv::sptr<T> v = slv::shared<T>(std::forward<args>(_args)...);

			if (!v->base_init(ctx))
			{
				v.reset();
				return nullptr;
			}

			return v;
		}

		void add(const slv::sptr<Vessel>& vessel);
		void remove(const slv::sptr<Vessel>& vessel);
		void destroy();
		size_t count() const;
		size_t count_active() const;
		size_t count_visible() const;
		float world_rotation() const;
		slv::vec2<float> world_position() const;
		slv::vec2<float> world_scale() const;
		slv::size<float> world_dimensions() const;

		inline float world_alpha() const
		{
			return m_world_alpha;
		}

		inline slv::size<float> dimensions() const
		{
			return dimensions_;
		}

		slv::vec2<float> anchor_pos() const
		{
			return slv::vec2<float>(dimensions_.width * anchor.x, dimensions_.height * anchor.y);
		}

		inline slv::wptr<Vessel> parent() const
		{
			return m_parent;
		}

		inline void set_name(const std::string& name)
		{
			m_name = name;
		}

		inline std::string name() const
		{
			return m_name;
		}

		inline virtual std::string type() const
		{
			return "Vessel";
		}

		slv::rgb color = slv::color::white;
		slv::vec2<float> pos{};
		slv::vec2<float> anchor{ 0.5f, 0.5f };
		slv::vec2<float> scale{ 1.f, 1.f };
		slv::vec2<float> skew{ 0.f, 0.f }; // degrees
		float rotation = 0.f; // degrees
		float alpha = 1.f; // 0 - 1
		float time_scale = 1.f;
		bool is_visible = true;
		bool is_active = true;

	protected:
		virtual void update(float dt, const slv::game_context& ctx) {}
		virtual void draw(const slv::game_context& ctx) const {}

		inline virtual bool init(const slv::game_context& ctx)
		{
			return true;
		}

		inline slv::mat3 world_transform() const
		{
			return m_world_transform;
		}

		slv::size<float> dimensions_;

	private:
		bool base_init(const slv::game_context& ctx);
		void base_update(float dt, const slv::game_context& ctx);
		void base_draw(const slv::game_context& ctx) const;
		bool has_ancestor(const slv::sptr<Vessel>& vessel) const;
		void clean_children();
		void mark_dirty();

		slv::wptr<Vessel> m_parent;
		std::vector<slv::sptr<Vessel>> m_children;
		std::string m_name;
		slv::mat3 m_local_transform = slv::mat3::identity();
		slv::mat3 m_world_transform = slv::mat3::identity();
		float m_world_alpha = 1.f;
		slv::size<float> m_last_dimensions{ -1.f, -1.f };
		slv::vec2<float> m_last_pos{ -1.f, -1.f };
		slv::vec2<float> m_last_anchor{ -1.f, -1.f };
		slv::vec2<float> m_last_scale{ -1.f, -1.f };
		slv::vec2<float> m_last_skew{ -1.f, -1.f };
		float m_last_rotation = -1.f;
		float m_last_alpha = -1.f;
		bool m_is_dirty = false;
		bool m_is_initialized = false;
	};
}