#pragma once

#include "Hand.h"

class Player :
	public Node
{
public:
	CREATE_FUNC(Player);
	bool init();

	void setPlayerDataWithUserInfo(COMMON::UserInfo userInfo);

	void clear();
	bool isActivated(){ return _isActivated; }

	void setAsPlayer();
	void setNameLabel(std::string name);
	void setPoke(std::string pokeFileName);
	void setPoke(int pokeNum);
	void setMoneyBet(int bet,int whole);

	unsigned getMoneyWhole() { return _money; }
	unsigned getMoneyBet() { return _betMoney; }

	void setCounter(float countTime);
	void initCounter();

	bool isAlreadyBet() { return _isAlreadyBet; }
	void setAlreadyBet(bool val) { _isAlreadyBet = val; }
	void setValueLabel(std::pair<int, int> values);

private:

public:
	Hand* _hand[2];

private:
	Sprite* _nameTag;
	Sprite* _nameTagBack;
	ProgressTimer* _timer;
	Sprite* _pokemon;
	Sprite* _stateBanner;
	Label* _nameLabel;
	Label* _moneyLabelFront;
	Label* _moneyLabelBack;
	
	Label* _valueLabel;
	

	unsigned _money;
	unsigned _betMoney;
	bool _isActivated = false;
	bool _isAlreadyBet = false;
	
};