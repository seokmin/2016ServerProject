#pragma once

#include "Hand.h"


class Dealer : public Node
{
public:
	CREATE_FUNC(Dealer);
	bool init() override;

private:

public:

private:
	Hand* _hand = nullptr;
	Label*	_pointLabel = nullptr;
};