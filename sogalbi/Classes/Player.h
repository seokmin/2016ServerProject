#pragma once

class Player :
	public Node
{
public:
	CREATE_FUNC(Player);
	bool init() override;

private:
	Sprite* _backGround;
	Sprite* _pokemon;
	Sprite* _banner;
	Label* _nameLabel;
	Label* _moneyLabelFront;
	Label* _moneyLabelBack;
	Label* _stateLabel;
	
	unsigned _money;
	unsigned _betMoney;
	bool _isActivated = false;
	
};