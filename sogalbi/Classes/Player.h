#pragma once

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

private:
	Sprite* _nameTag;
	Sprite* _nameTagBack;
	ProgressTimer* _timer;
	Sprite* _pokemon;
	Sprite* _stateBanner;
	Label* _nameLabel;
	Label* _moneyLabelFront;
	Label* _moneyLabelBack;
	
	unsigned _money;
	unsigned _betMoney;
	bool _isActivated = false;
	
};