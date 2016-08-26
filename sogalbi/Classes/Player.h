#pragma once

class Player :
	public Node
{
public:
	CREATE_FUNC(Player);
	bool init();

	void setPlayerDataWithUserInfo(COMMON::UserInfo userInfo);

	void setAsPlayer();
	void setNameLabel(std::string name);
	void setPoke(std::string pokeFileName);
	void setPoke(int pokeNum);
	void setMoneyWhole();
	void setMoneyBet(int bet,int whole);

private:
	Sprite* _nameTag;
	Sprite* _nameTagBack;
	Sprite* _pokemon;
	Sprite* _stateBanner;
	Label* _nameLabel;
	Label* _moneyLabelFront;
	Label* _moneyLabelBack;
	
	unsigned _money;
	unsigned _betMoney;
	bool _isActivated = false;
	
};