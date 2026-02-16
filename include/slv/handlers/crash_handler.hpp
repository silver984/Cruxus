#pragma once

#ifndef SLV_CRASH_HND
#define SLV_CRASH_HND slv::CrashHandler::get()
#endif

namespace slv
{
	class Game; // forward declare

	class CrashHandler
	{
		friend class slv::Game;

	private:
		CrashHandler() = default;
		~CrashHandler() = default;

		static inline CrashHandler& get()
		{
			static CrashHandler instance;
			return instance;
		}

		CrashHandler(const CrashHandler&) = delete;
		CrashHandler(CrashHandler&&) = delete;
		CrashHandler& operator = (const CrashHandler&) = delete;
		CrashHandler& operator = (CrashHandler&&) = delete;

		bool init();

		bool m_is_init = false;
	};
}