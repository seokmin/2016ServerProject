#pragma once
#include "ServerConfig.h";
class Logger;
class DBmanager
{
public:
	DBmanager();
	virtual ~DBmanager();
	COMMON::ERROR_CODE Init(Logger* logger);
	
	COMMON::ERROR_CODE SubmitState(int max, int count, ServerConfig* serverConfig);

private:
	Logger* m_logger;
};

