#pragma once

#include <atomic>
#include <cstdlib>
#include <cstdint>
#include <new>

namespace slv::heap
{
	namespace impl
	{
		std::atomic<size_t> allocated = 0;
	}

	float kb()
	{
		return impl::allocated / (1024.f);
	}

	float mb()
	{
		return impl::allocated / (1024.f * 1024.f);
	}

	float gb()
	{
		return impl::allocated / (1024.f * 1024.f * 1024.f);
	}
}

void* operator new(size_t size)
{
	if (void* ptr = std::malloc(size))
	{
		slv::heap::impl::allocated += size;
		return ptr;
	}

	throw std::bad_alloc();
}

void operator delete(void* ptr, size_t size) noexcept
{
	slv::heap::impl::allocated -= size;
	std::free(ptr);
}