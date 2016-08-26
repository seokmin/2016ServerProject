#include "pch.h"
#include "..\Common\Common.h"
#include "Player.h"
#include "ResourceInfo.h"

bool Player::init()
{
	_nameTag = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG);
	_nameTagBack = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG_BACK);
	_nameLabel = Label::createWithTTF("NULL", FILENAME::FONT::SOYANON,24);
	_pokemon = Sprite::create();
	

	addChild(_nameTag,1);
	addChild(_nameTagBack,0);
	addChild(_nameLabel,2);
	addChild(_pokemon,2);

	return true;
}

void Player::setPlayerDataWithUserInfo(COMMON::UserInfo userInfo)
{
	if (userInfo._name[0] == '\0')
		return;
	setPoke(userInfo._pokeNum);
	
	char utfChar[1024];
	int nLen = WideCharToMultiByte(CP_UTF8, 0, userInfo._name, lstrlen(userInfo._name), nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, userInfo._name, lstrlen(userInfo._name), utfChar, nLen, nullptr, nullptr);
	setNameLabel(utfChar);
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
