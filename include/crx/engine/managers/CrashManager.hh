#pragma once
#include <crx/config.hh>

namespace crx {

class Game; // forward declare
class CRX_DLL CrashManager final {
	friend class Game;

private:
	CrashManager();
	~CrashManager();

public:
	CrashManager(CrashManager const&) = delete;
	CrashManager(CrashManager&&) = delete;
	CrashManager& operator=(CrashManager const&) = delete;
	CrashManager& operator=(CrashManager&&) = delete;

private:
	bool init();
	bool is_initialized_;
};

}