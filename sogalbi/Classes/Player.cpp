#include "pch.h"
#include "..\Common\Common.h"
#include "Player.h"
#include "ResourceInfo.h"
#include "ClientLogger.h"

bool Player::init()
{
	_nameTag = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG);
	_nameTagBack = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG_BACK);
	_nameLabel = Label::createWithTTF("NULL", FILENAME::FONT::SOYANON,24);
	_pokemon = Sprite::create();

	_moneyLabelFront = Label::createWithTTF(u8"0", FILENAME::FONT::SOYANON, 24);
	_moneyLabelBack = Label::createWithTTF(u8"/0", FILENAME::FONT::SOYANON, 24);
	

	addChild(_nameTag,1);
	addChild(_nameTagBack,0);
	addChild(_nameLabel,2);
	addChild(_pokemon, 2);
	addChild(_moneyLabelFront, 2);
	addChild(_moneyLabelBack, 2);

	_moneyLabelFront->setAnchorPoint(Vec2(1, 1));
	_moneyLabelBack->setAnchorPoint(Vec2(0, 1));
	_moneyLabelFront->setPosition(25, 0);
	_moneyLabelBack->setPosition(25, 0);

	_nameLabel->setAnchorPoint(Vec2(0, 0));
	_nameLabel->setPosition(-45, 0);

	_pokemon->setPosition(-80.f, 0.f);

	return true;
}

void Player::setPlayerDataWithUserInfo(COMMON::UserInfo userInfo)
{
	if (userInfo._name[0] == '\0')
		return;
	setPoke(userInfo._pokeNum);
	
	char utfChar[128];
	ZeroMemory(utfChar, sizeof(utfChar));
	int nLen = WideCharToMultiByte(CP_UTF8, 0, userInfo._name, lstrlen(userInfo._name), nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, userInfo._name, lstrlen(userInfo._name), utfChar, nLen, nullptr, nullptr);
	setNameLabel(utfChar);

	setMoneyBet(userInfo._betMoney, userInfo._totalMony);
}

void Player::setAsPlayer()
{
	_nameTagBack->setColor(Color3B::YELLOW);
	_nameLabel->setColor(Color3B::YELLOW);
}

void Player::setNameLabel(std::string name)
{
	_nameLabel->setString(name);
}

void Player::setPoke(std::string pokeFileName)
{
	_pokemon->setSpriteFrame(pokeFileName);
}

void Player::setPoke(int pokeNum)
{
	if (pokeNum == 0)
		return;
	_pokemon->setSpriteFrame(FILENAME::SPRITE::POKE_ARRAY[pokeNum]);
}

void Player::setMoneyBet(int bet,int whole)
{
	_betMoney = bet;
	_money = whole;
	_moneyLabelFront->setString(std::to_string(bet));
	_moneyLabelBack->setString(u8"/"+std::to_string(whole));
}
