#pragma once
#include <memory>
#include <utility>

namespace slv {

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using wptr = std::weak_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T, typename... Args>
sptr<T> shared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
uptr<T> unique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

}