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

void Hand::pushCard(CardInfo& card)
{
	_cardInfos[_handNum] = card;

	auto& cardSpr = _cardSprites[_handNum];
	auto spawn = Spawn::create(MoveTo::create(0.3, cardSpr->getPosition()), RotateTo::create(0.3, 0), nullptr);
	cardSpr->setPosition(cardSpr->getPosition() + Vec2(0,100));
	cardSpr->setRotation(180);
	
	// 해당 카드 모양에 맞는 스프라이트로 설정
	auto sprFrame = SpriteFrameCache::getInstance()->
		getSpriteFrameByName(FILENAME::SPRITE::CARD_ARRAY[card._shape][card._number]);
	cardSpr->setSpriteFrame(sprFrame);
	cardSpr->setVisible(true);
	cardSpr->runAction(spawn);
	++_handNum;
}

std::pair<int, int> Hand::getHandValue()
{
	std::pair<int, int> value = { 0,0 };
	for (auto& card : _cardInfos)
	{
		if (card._shape == CardInfo::CardShape::EMPTY)
			break;
		if (card._number == 1)
		{
			value.first += 1;
			value.second += 11;
		}
		else if (card._number == 11 || card._number == 12 || card._number == 13) // jack
		{
			value.first += 10;
			value.second += 10;
		}
		else
		{
			value.first += card._number;
			value.second += card._number;
		}
	}
	return value;
}

void Hand::clear()
{
	for (auto& card : _cardInfos)
	{
		card._shape = CardInfo::CardShape::EMPTY;
		card._number = 0;
	}
	for (auto& spr : _cardSprites)
	{
		spr->setVisible(false);
	}
}
