#pragma once

#include <memory>
#include <utility>

namespace slv
{
	template<typename T>
	using sptr = std::shared_ptr<T>;

	template<typename T>
	using wptr = std::weak_ptr<T>;

	template<typename T>
	using uptr = std::unique_ptr<T>;

	template<typename T, typename... args>
	inline sptr<T> shared(args&&... _args)
	{
		return std::make_shared<T>(std::forward<args>(_args)...);
	}

	template<typename T, typename... args>
	inline uptr<T> unique(args&&... _args)
	{
		return std::make_unique<T>(std::forward<args>(_args)...);
	}
}