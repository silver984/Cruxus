#pragma once
#include <slv/internal/config.hpp>

namespace slv {

class App; // forward declare
class SLV_DLL CrashManager final {
	friend class App;

private:
	CrashManager();
	~CrashManager();

public:
	CrashManager(CrashManager const&) = delete;
	CrashManager& operator=(CrashManager const&) = delete;
	CrashManager(CrashManager&&) = delete;
	CrashManager& operator=(CrashManager&&) = delete;

private:
	bool init();
	bool is_initialized_;
};

}