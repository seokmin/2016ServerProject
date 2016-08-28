#include "stdafx.h"
#include "Dealer.h"
#include "Room.h"
#include "User.h"

void Dealer::Init(Room* room)
{
	m_deck.Init();
	m_pRoom = room;

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
}

void Dealer::Clear()
{
	m_deck.Clear();
	m_pRoom = nullptr;
	m_curCardNum = 0;
}

void Dealer::SetHand(COMMON::CardInfo card)
{
	m_dealerHand._cardList[m_curCardNum]._shape = card._shape;
	m_dealerHand._cardList[m_curCardNum]._number = card._number;
	++m_curCardNum;
}
