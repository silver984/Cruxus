#pragma once

#include <memory>

template<typename T>
using s_ptr = std::shared_ptr<T>;

template<typename T>
using w_ptr = std::weak_ptr<T>;

template<typename T>
using u_ptr = std::unique_ptr<T>;

template<typename T, typename... args>
inline s_ptr<T> shared(args&&... _args)
{
	return std::make_shared<T>(std::forward<args>(_args)...);
}

template<typename T, typename... args>
inline u_ptr<T> unique(args&&... _args)
{
	return std::make_unique<T>(std::forward<args>(_args)...);
}