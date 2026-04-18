#include <crx/engine/main/backend/CrashManager.hh>
#include <crx/engine/log.hh>
#ifdef _WIN32
#include "platforms/windows.hh"
#endif

namespace crx {

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