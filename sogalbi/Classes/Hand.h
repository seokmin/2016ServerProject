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

private:
public:
	CardInfo _cardInfos[7];
private:
	Sprite* _cardSprites[7];
	int		_handNum = 0;
};