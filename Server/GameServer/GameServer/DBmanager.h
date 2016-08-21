#pragma once
#include "ServerConfig.h";

class DBmanager
{
public:
	DBmanager();
	virtual ~DBmanager();
	COMMON::ERROR_CODE Init();
	
	COMMON::ERROR_CODE SubmitState(int max, int count, ServerConfig* serverConfig);

private:
};

