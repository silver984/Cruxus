#pragma once

namespace slv
{
	class CrashHandler
	{
	private:
		CrashHandler() = default;
		~CrashHandler() = default;

	public:
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

	private:
		bool m_is_init = false;
	};
}