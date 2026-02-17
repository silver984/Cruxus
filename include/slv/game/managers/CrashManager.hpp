#pragma once

namespace slv
{
	class Game; // forward declare

	class CrashManager
	{
		friend class slv::Game;

	private:
		CrashManager() = default;
		~CrashManager() = default;
		CrashManager(const CrashManager&) = delete;
		CrashManager& operator=(const CrashManager&) = delete;
		CrashManager(CrashManager&&) = delete;
		CrashManager& operator=(CrashManager&&) = delete;

		bool init();

		static constexpr inline const char* M_NAME = "CrashHandler";
		bool m_is_init = false;
	};
}