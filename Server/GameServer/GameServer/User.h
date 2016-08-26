#pragma once

#include "ServerConfig.h"
#include "Common.h"

enum class DOMAIN_STATE
{
	NONE = 0,
	LOGIN = 1,
	ROOM = 2,
};

enum class IO_STATE
{
	NONE = 0,
	ING = 1
};

class User
{
public:

public:
	User(const int i) : m_userId(i) {};
	virtual ~User() {};

	void Init(std::string authToken, std::wstring userName, int pokeNum, int totalMoney);
	void SetSessionIndex(int sessionIndex) { m_sessionIndex = sessionIndex; };
	void Clear();
	void EnterRoom(int roomIdx);
	void LeaveRoom();

	COMMON::UserInfo GetUserInfo();

	bool				IsAvailableFromPool() { return m_sessionIndex < 0; };
	bool				CheckUserWithAuthToken(std::string authToken) { return (m_authToken == authToken); };
	bool				CheckUserWithSessionIndex(int sessionIndex) { return (m_sessionIndex == sessionIndex); };
	int					GetCurRoomIdx() { return m_currentRoomIdx; };
	int					GetUserIdx() { return m_userId; };
	int					GetPokeNum() { return m_pokeNum; };
	std::wstring		GetName() { return m_userName; };
	int					GetTotalMoney() { return m_totalMoney; };
	int					GetSessionIndex() { return m_sessionIndex; };
	bool				IsCurDomainRoom() { return m_curDomain == DOMAIN_STATE::ROOM; };
	bool				IsCurDomainLogin() { return m_curDomain == DOMAIN_STATE::LOGIN; };
	bool				IsIoState() { return m_ioState == IO_STATE::ING; }
	void				SetIoState(IO_STATE state) { m_ioState = state; }

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

	DOMAIN_STATE	m_curDomain = DOMAIN_STATE::NONE;
	IO_STATE		m_ioState = IO_STATE::NONE;

	int m_money = 0;
};

