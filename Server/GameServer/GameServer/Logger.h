#pragma once
#include "Common.h"

class Logger
{
public:
	Logger();
	virtual ~Logger();
	
	enum Level {
		INFO,
		DEBUG,
		WARNING,
		EXCEPTION,
		ERROR_NORMAL,
		ERROR_SIMPLE,
	};


	void Log(Level level, WCHAR* message, int lessageLen);
private:
};

