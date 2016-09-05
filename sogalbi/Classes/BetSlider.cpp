#include "pch.h"

#include "ResourceInfo.h"

#include "BetSlider.h"

bool BetSlider::init()
{
	if (Slider::init() == false)
		return false;


	// 슬라이더 구성
	auto trackFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(FILENAME::SPRITE::SLIDER_TRACK);
	auto progressFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(FILENAME::SPRITE::SLIDER_PROGRESS);
	auto ballFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(FILENAME::SPRITE::SLIDER_THUMB);

	this->loadBarTexture(trackFrame);
	this->loadProgressBarTexture(progressFrame);
	this->loadSlidBallTextureNormal(ballFrame);
	this->loadSlidBallTexturePressed(ballFrame);
	this->loadSlidBallTextureDisabled(ballFrame);

	this->addEventListenerSlider(this, sliderpercentchangedselector(BetSlider::SliderChanged));

	// 라벨 구성
	_minLabel = Label::createWithTTF("", FILENAME::FONT::SOYANON, 36);
	_maxLabel = Label::createWithTTF("", FILENAME::FONT::SOYANON, 36);
	_currentLabel = Label::createWithTTF("", FILENAME::FONT::SOYANON, 36);
	_currentLabel->setColor(Color3B::GREEN);

	this->addChild(_minLabel);
	this->addChild(_maxLabel);
	this->addChild(_currentLabel);

	_minLabel->setPosition(0, 30);
	_maxLabel->setPosition(980, 30);
	_currentLabel->setPositionY(41);

}

BetSlider* BetSlider::create(int minBet, int maxBet)
{
	auto instance = BetSlider::create();
	instance->setMinBet(minBet);
	instance->setMaxBet(maxBet);
	instance->_currentBet = minBet;

	instance->_currentLabel->setString(std::to_string(instance->_currentBet));
	float currentPos = 2;
	instance->_currentLabel->setPositionX(currentPos);

	return instance;
}

void BetSlider::setMinBet(int value)
{
	_minBet = value;
	_minLabel->setString(std::to_string(value));
}

void BetSlider::setMaxBet(int value)
{
	_maxBet = value;
	_maxLabel->setString(std::to_string(value));
	SliderChanged(nullptr, SLIDER_PERCENTCHANGED);
}

void BetSlider::SliderChanged(Ref* sender, SliderEventType type)
{
	if (type == SliderEventType::SLIDER_PERCENTCHANGED)
	{
		int percent = getPercent();
		if (percent == 0)
			return;
		float divVal = (float)percent / 100.f;
		_currentBet= (_maxBet - _minBet) * divVal + _minBet;
		_currentLabel->setString(std::to_string(_currentBet));
		float currentPos = percent * 980 / 100 + 2;
		_currentLabel->setPositionX(currentPos);
	}
}
