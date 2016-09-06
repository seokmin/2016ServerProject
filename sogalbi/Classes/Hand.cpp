#include "pch.h"

#include "ResourceInfo.h"

#include "Hand.h"

bool Hand::init()
{
	for (int i = 0; i < 7; ++i)
	{
		_cardSprites[i] = Sprite::create();
		addChild(_cardSprites[i], i);
	}
	_cardSprites[0]->setPosition(0, 40);
	_cardSprites[1]->setPosition(40, 40);
	_cardSprites[2]->setPosition(80, 40);
	_cardSprites[3]->setPosition(120, 40);
	_cardSprites[4]->setPosition(0, 0);
	_cardSprites[5]->setPosition(50, 0);
	_cardSprites[6]->setPosition(90, 0);
	clear();
	return true;
}

void Hand::pushCard(CardInfo& card, float delay /*= 0.f*/)
{
	if (card._number == 0 || card._shape == CardInfo::CardShape::EMPTY)
		return;

	_cardInfos[_handNum] = card;

	auto& cardSpr = _cardSprites[_handNum];
	auto spawn = Spawn::create( MoveTo::create(0.3f, cardSpr->getPosition()), RotateTo::create(0.3f, 0), nullptr);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::setVisible, cardSpr, true));
	auto dieFunc = CallFunc::create(CC_CALLBACK_0(Hand::Die, this, .5f));
	if (getHandValue().first <= 21)
		dieFunc = nullptr;
	auto seq = Sequence::create(DelayTime::create(delay),callFunc, spawn,dieFunc, nullptr);
	cardSpr->setPosition(cardSpr->getPosition() + Vec2(0,100));
	cardSpr->setRotation(180);
	
	// 해당 카드 모양에 맞는 스프라이트로 설정
	auto sprFrame = SpriteFrameCache::getInstance()->
		getSpriteFrameByName(FILENAME::SPRITE::CARD_ARRAY[card._shape][card._number]);
	cardSpr->setSpriteFrame(sprFrame);
	cardSpr->setVisible(false);
	cardSpr->runAction(seq);
	++_handNum;
}

void Hand::popCard()
{
	--_handNum;
	_cardSprites[_handNum]->setVisible(false);
}

std::pair<int, int> Hand::getHandValue()
{
	std::pair<int, int> valuePair = { 0,0 };
	int value = 0;
	for (auto& card : _cardInfos)
	{
		if (card._shape == CardInfo::CardShape::EMPTY)
			break;
		if (card._number == 1)
		{
			value += 1;
		}
		else if (card._number == 11 || card._number == 12 || card._number == 13) // jack
		{
			value += 10;
		}
		else
		{
			value += card._number;
		}
	}

	valuePair.first = value;
	valuePair.second = value;;
	for(auto& card : _cardInfos)
		if (card._number == 1)
		{
			valuePair.second = value + 10;
			break;
		}

	if (valuePair.second > 21)
		valuePair.second = valuePair.first;
	if (valuePair.second == 21 || valuePair.first == 21)
		valuePair = { 21,21 };
	
	return valuePair;
}

void Hand::clear()
{
	_handNum = 0;
	for (auto& card : _cardInfos)
	{
		card._shape = CardInfo::CardShape::EMPTY;
		card._number = 0;
	}
	for (auto& spr : _cardSprites)
	{
		spr->setVisible(false);
		spr->setColor(Color3B(255, 255, 255));
	}
}

void Hand::Die(float delay)
{
	auto darkenFunc = [&]() {
		for (auto& spr : _cardSprites)
		{
			if (spr != nullptr)
			{
				spr->setColor(Color3B(50, 50, 50));
			}
		}
	};
	auto callFunc = CallFunc::create(darkenFunc);
	auto seq = Sequence::create(DelayTime::create(delay), callFunc, nullptr);
	this->runAction(seq);
}
