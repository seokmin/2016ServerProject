#include "pch.h"

#include "ResourceInfo.h"	

#include "Dealer.h"

bool Dealer::init()
{
	// 점수 라벨
	_valueLabel = Label::createWithTTF("", FILENAME::FONT::SOYANON, 48);
	_valueLabel->setColor(Color3B::YELLOW);
	_valueLabel->setPositionX(-100);
	addChild(_valueLabel);
	
	// 핸드
	_hand = Hand::create();
	addChild(_hand);

	return true;
}

void Dealer::playDealerResultAnimation(DealerInfo& dealerInfo)
{
	auto& cardList = dealerInfo._openedCardList;

	float waitingTime = 0.f;
	for (auto& card : cardList)
	{
		if (card._shape == CardInfo::CardShape::EMPTY)
			break;
		auto copiedCard = card;
		auto callFunc = CallFunc::create(CC_CALLBACK_0(Dealer::pushCard, this,copiedCard));
		auto seq = Sequence::createWithTwoActions(DelayTime::create(waitingTime), callFunc);
		runAction(seq);
		waitingTime += 1.f;
	}


}

void Dealer::pushCard(CardInfo& card)
{
	_hand->pushCard(card);
	_valueLabel->setString(std::to_string(_hand->getHandValue().second));
}

void Dealer::clear()
{
	_hand->clear();
	_valueLabel->setString("");
}
