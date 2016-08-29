#pragma once

#include "Common.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <queue>

class Room;

class Dealer
{
struct Deck
{
	void Init()
	{
		for (int type = 1; type <= 4; ++type)
		{
			for (int num = 1; num <= 13; ++num)
			{
				COMMON::CardInfo card;
				card._shape = (COMMON::CardInfo::CardShape)type;
				card._number = num;
				_deck.push_back(card);
			}
		}
		std::srand(unsigned(std::time(0)));
		std::random_shuffle(_deck.begin(), _deck.end());
	}

	void Clear()
	{
		_deck.clear();
	}

	COMMON::CardInfo Draw()
	{
		auto card = _deck.front();
		_deck.pop_front();

		return card;
	}

	std::deque<COMMON::CardInfo> _deck;
};

public:
	Dealer() {};
	~Dealer() {};

	void Init(Room* room);
	void Clear();

	COMMON::CardInfo Draw() { return m_deck.Draw(); };

	COMMON::HandInfo GetHand() { return m_dealerHand; };
	COMMON::CardInfo GetCard(int num) { return m_dealerHand._cardList[num]; };

private:
	void SetHand(COMMON::CardInfo card);

private:
	Room* m_pRoom;

	COMMON::HandInfo m_dealerHand;
	Deck m_deck;

	int m_curCardNum = 0;

};