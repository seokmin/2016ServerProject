#pragma once

#include "ServerConfig.h"
#include "Common.h"

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

	void Init(int sessionIndex, std::string authToken, std::wstring userName, int pokeNum, int totalMoney);
	void Clear();
	void EnterRoom(int roomIdx);

	COMMON::UserInfo GetUserInfo();

	bool IsAvailableFromPool() { return (m_sessionIndex < 0); };
	bool CheckUserWithAuthToken(std::string authToken) { return (m_authToken == authToken); };
	bool CheckUserWithSessionIndex(int sessionIndex) { return (m_sessionIndex == sessionIndex); };
	int  GetCurRoomIdx() { return m_currentRoomIdx; };
	int  GetUserIdx() { return m_userId; };
	int	 GetPokeNum() { return m_pokeNum; };
	std::wstring		GetName() { return m_userName; };
	int					GetTotalMoney() { return m_totalMoney; };
	int					GetSessionIndex() { return m_sessionIndex; };

private:
	void InitHand();

private:
	int m_userId = -1;
	int m_sessionIndex = -1;
	int m_currentRoomIdx = -1;
	std::string m_authToken = "";
	std::wstring m_userName = L"";
	int m_pokeNum = 0;
	int m_totalMoney = 0;
	int m_curBetMoney = 0;

	COMMON::HandInfo m_hand[MAX_HAND];

	DOMAIN_STATE m_curDomain = DOMAIN_STATE::NONE;
};

