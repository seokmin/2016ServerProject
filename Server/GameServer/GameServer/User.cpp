#include "stdafx.h"
#include "User.h"

void User::Init(std::string authToken, std::wstring userName, int pokeNum, int totalMoney)
{
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
	m_currentRoomIdx = -1;
	m_authToken = "";
	m_userName = L"";
	m_pokeNum = 0;
	m_totalMoney = 0;
	m_curBetMoney = 0;
	m_curSeat = 0;
	m_curCardNum = 0;

	InitHand();

	m_curDomain = DOMAIN_STATE::NONE;
	m_gameState = GAME_STATE::NONE;
	m_ioState = IO_STATE::NONE;
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
	m_gameState = GAME_STATE::WAITING;
	m_currentRoomIdx = roomIdx;
}

void User::LeaveRoom()
{
	m_currentRoomIdx = -1;
	m_curDomain = DOMAIN_STATE::LOGIN;
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

ERROR_CODE User::ApplyBet(int betMoney)
{
	if (betMoney > m_totalMoney)
	{
		WCHAR errStr[100];
		wsprintf(errStr, L"유저(%s)가 자기 돈 보다 많은 배팅을 했어..", m_userName);
		Logger::GetInstance()->Log(Logger::ERROR_NORMAL, errStr, 100);

		return ERROR_CODE::ROOM_GAME_NOT_ENOUGH_MONEY;
	}

	m_totalMoney -= betMoney;
	m_curBetMoney = betMoney;
	m_gameState = GAME_STATE::BET_DONE;

	return ERROR_CODE::NONE;
}

void User::SetHand(int hand, CardInfo card)
{
	m_hand[hand]._cardList[m_curCardNum]._shape = card._shape;
	m_hand[hand]._cardList[m_curCardNum]._number = card._number;
	++m_curCardNum;
}
