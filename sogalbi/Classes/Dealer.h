#pragma once

#include "Hand.h"


class Dealer : public Node
{
public:
	CREATE_FUNC(Dealer);
	bool init() override;
	void playDealerResultAnimation(DealerInfo& dealerInfo);
	void pushCard(CardInfo& card);
	void clear();

private:

public:

private:
	Hand* _hand = nullptr;
	Label*	_valueLabel = nullptr;
	int		_currentValue = 0;
};