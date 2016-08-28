#pragma once
#include "..\Common\Packet.h"
#include <utility>


using namespace COMMON;

class Hand : public Node
{
public:
	CREATE_FUNC(Hand);
	virtual bool init() override;

	void pushCard(CardInfo& card);
	std::pair<int, int> getHandValue();
	void clear();

private:
public:
private:
	Sprite* _cardSprites[7];
	CardInfo _cardInfos[7];
	int		_handNum = 0;
};