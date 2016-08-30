#pragma once

#include "ui/UISlider.h"

using namespace ui;

class BetSlider : public Slider
{
public:
	virtual bool init();
	CREATE_FUNC(BetSlider);
	static BetSlider* create(int minBet, int maxBet, int startPercent = 10);

	void setMinBet(int value);
	void setMaxBet(int value);

	int getCurrentBet() { return _currentBet; }

	//void setPercent(int value) override;

private:
	void SliderChanged(Ref* sender, SliderEventType type);
public:
private:
	Label* _minLabel;
	Label* _maxLabel;
	Label* _currentLabel;

	int		_minBet = 1;
	int		_maxBet = 100;
	int		_currentBet;
};