#include "stdafx.h"
#include "User.h"

void User::Init(int sessionIndex, std::string authToken, std::wstring userName, int pokeNum, int totalMoney)
{
	m_sessionIndex = sessionIndex;
	m_authToken = authToken;
	m_curDomain = DOMAIN_STATE::LOGIN;
	m_userName = userName;
	m_pokeNum = pokeNum;
	m_totalMoney = totalMoney;
	InitHand();
}

void User::Clear()
{
	m_sessionIndex = -1;
	m_authToken = "";
	m_currentRoomIdx = -1;
	m_curDomain = DOMAIN_STATE::NONE;
	m_userName = L"";
	m_pokeNum = 0;
	m_totalMoney = 0;
	m_curBetMoney = 0;
	InitHand();
}

void User::InitHand()
{
	for (int i = 0; i < MAX_HAND; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			m_hand[i]._cardList[j]._shape = COMMON::CardInfo::CardShape::EMPTY;
			m_hand[i]._cardList[j]._number = 0;
		}
		m_hand[i]._handState = COMMON::HandInfo::CURRENT;
		m_hand[i]._isDoubledown = false;
	}
}

void User::EnterRoom(int roomIdx)
{
	m_curDomain = DOMAIN_STATE::ROOM;
	m_currentRoomIdx = roomIdx;
}

COMMON::UserInfo User::GetUserInfo()
{
	COMMON::UserInfo userInfo;

	userInfo._pokeNum = m_pokeNum;
	wcscpy_s(userInfo._name, _countof(userInfo._name), m_userName.c_str());
	userInfo._totalMony = m_totalMoney;
	userInfo._betMoney = m_curBetMoney;
	for (int i = 0; i < MAX_HAND; ++i)
	{
		userInfo._hands[i] = m_hand[i];
	}

	return userInfo;
}
