#pragma once

class Player :
	public Node
{
public:
	CREATE_FUNC(Player);
	bool init() override;
	void setAsPlayer();
	void setNameLabel(std::string name);
	void setPoke(std::string pokeFileName);
	void setMoneyWhole();
	void setMoneyBet();

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