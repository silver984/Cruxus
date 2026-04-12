#include <slv/engine/game/managers/CrashManager.hpp>
#include <slv/engine/log.hpp>
#ifdef _WIN32
#include <platforms/windows.hpp>
#endif

namespace slv {

// private
CrashManager::CrashManager() :
	is_initialized_(false)
{}

// private
CrashManager::~CrashManager() = default;

// private
bool CrashManager::init() {
	if (is_initialized_) {
		return true;
	}

#ifdef _WIN32
	win32::init_mem_dump();
	is_initialized_ = true;
	return true;
#endif

	return false;
}

}