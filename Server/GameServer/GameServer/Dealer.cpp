#include "stdafx.h"
#include "Dealer.h"
#include "Room.h"
#include "User.h"

void Dealer::Init(Room* room)
{
	m_pRoom = room;
	m_deck.Init();

	bool flag = true;
	for (int handCnt = 0; handCnt < 2; ++handCnt)
	{
		for (int i = 0; i < 5; ++i)
		{
			User* user = m_pRoom->GetUserInfo(i);
			if (user == nullptr) continue;

			user->SetHand(0, m_deck.Draw());

			if (flag)
			{
				// 카지노 보니까 딜러는 한 장만 들고있음..
				flag = false;
				SetHand(m_deck.Draw());
			}
		}
	}

	// 블랙젝인지 체크하고, 블랙젝이면 핸드 상태를 바꿔줌
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		User* user = m_pRoom->GetUserInfo(i);
		if (user == nullptr) continue;

		if (std::get<1>(user->GetCardSum(0)) == 21)
		{
			user->SetHandState(0, COMMON::HandInfo::HandState::BLACKJACK);
		}
	}
}

void Dealer::Clear()
{
	m_deck.Clear();
	m_pRoom = nullptr;
	m_curCardNum = 0;
	m_dealerHand.Reset();
}

void Dealer::SetHand(COMMON::CardInfo card)
{
	m_dealerHand._cardList[m_curCardNum]._shape = card._shape;
	m_dealerHand._cardList[m_curCardNum]._number = card._number;
	++m_curCardNum;
}

int Dealer::GetCardSum()
{
	/*int sum = 0;

	bool flag = false;
	for (int i = 0; i < m_curCardNum; ++i)
	{
		sum += m_dealerHand._cardList[i]._number;
		if (m_dealerHand._cardList[i]._number > 10)
		{
			sum -= (m_dealerHand._cardList[i]._number - 10);
		}

		if (m_dealerHand._cardList[i]._number == 1)
		{
			flag = true;
		}

	}

	if (flag)
	{
		if (sum + 10 <= 21)
			sum += 10;
	}*/
	auto resultPair = m_dealerHand.GetScore();
	int sum1 = std::get<0>(resultPair);
	int sum2 = std::get<1>(resultPair);

	if (sum2 > 21)
		return sum1;
	else
		return sum2;
}
