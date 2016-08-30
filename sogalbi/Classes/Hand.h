#pragma once
#include "..\Common\Packet.h"
#include <utility>


using namespace COMMON;

class Hand : public Node
{
public:
	CREATE_FUNC(Hand);
	virtual bool init() override;

	void pushCard(CardInfo& card, float delay = 0.f);
	std::pair<int, int> getHandValue();
	void clear();
	void Die(float delay);
	
private:
	static const int MAX_CARD = 7;

public:
	CardInfo _cardInfos[MAX_CARD];
private:
	Sprite* _cardSprites[MAX_CARD];
	int		_handNum = 0;
};