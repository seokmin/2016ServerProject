#pragma once

#include "ServerConfig.h"

class User
{
public:
	enum class DOMAIN_STATE
	{
		NONE = 0,
		LOGIN = 1,
		ROOM = 2,
	};

public:
	User(const int i) : m_userId(i) {};
	virtual ~User() {};

	void Init(int sessionIndex, std::string authToken);
	void Clear();

	bool IsAvailableFromPool() { return (m_sessionIndex < 0); };
	bool CheckUserWithAuthToken(std::string authToken) { return (m_authToken == authToken); };
	int  GetCurRoomIdx() { return m_currentRoomIdx; };
	int  GetUserIdx() { return m_userId; };
	
private:
	int m_userId = -1;
	int m_sessionIndex = -1;
	int m_currentRoomIdx = -1;
	std::string m_authToken = "";

	DOMAIN_STATE m_curDomain = DOMAIN_STATE::NONE;
};

