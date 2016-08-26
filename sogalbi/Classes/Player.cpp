#include "pch.h"
#include "Player.h"
#include "ResourceInfo.h"

bool Player::init()
{
	auto runwith = [=]() {
		_nameTag = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG);
		_nameTagBack = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::NAMETAG_BACK);
		_nameLabel = Label::createWithTTF("NULL", FILENAME::FONT::SOYANON, 24);
		_pokemon = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::POKE_ARRAY[1]);
		addChild(_nameTag, 1);
		addChild(_nameTagBack, 0);
		addChild(_nameLabel, 2);
		addChild(_pokemon, 2);
	};
	Director::getInstance()->getScheduler()->performFunctionInCocosThread(runwith);

	return true;
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
