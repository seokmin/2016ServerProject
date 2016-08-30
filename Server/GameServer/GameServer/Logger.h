#pragma once
#include <mutex>

class Logger
{
public:
	
	enum Level {
		INFO,
		DEBUG,
		WARNING,
		EXCEPTION,
		ERROR_NORMAL,
		ERROR_FATAL,
	};

	void Log(Level level, WCHAR* message, int lessageLen);
	void Logf(Level level, WCHAR* message, ... );

	static Logger* GetInstance();
	static void DelInstance();

private:
	static Logger* g_instance;
	Logger(); //singleton
	virtual ~Logger();


private:
	WCHAR m_levelStr[6][20];
	std::mutex m_mutex;
};

