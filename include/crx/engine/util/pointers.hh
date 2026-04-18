#pragma once
#include <memory>
#include <utility>

namespace crx {
template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using wptr = std::weak_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T, typename... va_args>
sptr<T> shared(va_args&&... args) {
	return std::make_shared<T>(std::forward<va_args>(args)...);
}

template<typename T, typename... va_args>
uptr<T> unique(va_args&&... args) {
	return std::make_unique<T>(std::forward<va_args>(args)...);
}
}