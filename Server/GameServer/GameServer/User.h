#pragma once

#include "ServerConfig.h"

class User
{
public:
	User(const int i) : m_userId(i) {};
	virtual ~User() {};

private:
	int m_userId = -1;
	int m_sessionIndex = -1;
};

