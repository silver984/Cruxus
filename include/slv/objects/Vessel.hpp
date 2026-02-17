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
	class Vessel : public std::enable_shared_from_this<Vessel>
	{
	public:
		Vessel() = default;
		virtual ~Vessel() = default;

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

		inline size_t count() const
		{
			return m_vessels.size();
		}

		inline slv::size<float> size() const
		{
			return size_;
		}

		inline slv::size<float> scaled_size() const
		{
			return slv::size<float>(size_.width * world_scale_.x, size_.height * world_scale_.y);
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

		slv::vec2<float> pos{};
		slv::vec2<float> anchor{ 0.5F, 0.5F };
		slv::vec2<float> scale{ 1.0F, 1.0F };
		slv::rgb color = slv::color::white;
		float time_scale = 1.0F;
		float rotation = 0.0F;
		float alpha = 1.0F;
		bool is_visible = true;
		bool is_active = true;

	protected:
		inline virtual bool init(const slv::game_context& ctx)
		{
			return true;
		}

		virtual void update(float dt, const slv::game_context& ctx) {}
		virtual void draw(const slv::game_context& ctx) const {}
		bool base_init(const slv::game_context& ctx);
		void base_update(float dt, const slv::game_context& ctx);
		void base_draw(const slv::game_context& ctx) const;

		slv::size<float> size_;
		slv::vec2<float> world_scale_{ 1.0F, 1.0F };
		slv::vec2<float> world_pos_{};
		slv::vec2<float> world_anchor_{ 0.5F, 0.5F };
		float world_rotation_ = 0.0F;
		float world_alpha_ = 1.0F;

	private:
		bool has_ancestor(const slv::sptr<Vessel>& vessel) const;

		slv::wptr<Vessel> m_parent;
		std::vector<slv::sptr<Vessel>> m_vessels;
		std::string m_name;
		bool m_is_init = false;
	};
}