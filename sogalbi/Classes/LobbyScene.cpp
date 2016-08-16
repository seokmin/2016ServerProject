#include "pch.h"
#include "LobbyScene.h"
#include "ResourceInfo.h"
#include "ui\UITextField.h"
#include "network\HttpRequest.h"
#include "network\HttpClient.h"

#pragma execution_character_set("UTF-8")

cocos2d::Scene* LobbyScene::createScene(DEF::ChannelInfo info)
{
	auto scene = Scene::create();
	auto layer = LobbyScene::create(info);
	scene->addChild(layer);

	return scene;
}

bool LobbyScene::init()
{
	if (!Layer::init())
		return false;

	initLayout();

	return true;
}

void LobbyScene::menuCloseCallback(cocos2d::Ref* pSender)
{
	// 이전 씬으로 돌아간다.
	Director::getInstance()->popScene();
}

LobbyScene* LobbyScene::create(DEF::ChannelInfo info)
{
	LobbyScene *pRet = new(std::nothrow) LobbyScene();
	pRet->_channelInfo = info;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void LobbyScene::initLayout()
{
	// 배경
	auto bg = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::LOBBY_BG);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->getTexture()->setAliasTexParameters();
	addChild(bg);

	// 나가기 버튼
	auto leaveLabel = Label::createWithTTF("나가기", FILENAME::FONT::SOYANON, 40);
	leaveLabel->setColor(Color3B(201, 201, 201));
	auto leaveButton = MenuItemLabel::create(leaveLabel, CC_CALLBACK_1(LobbyScene::menuCloseCallback, this));
	auto leaveMenu = Menu::create(leaveButton, nullptr);
	leaveMenu->setPosition(1199.f, 674.f);
	addChild(leaveMenu, DEF::Z_ORDER::UI);

	// 좌측 상단 라벨
	_serverLabel = Label::createWithTTF(_channelInfo.name, FILENAME::FONT::SOYANON, 64);
	_serverLabel->setColor(Color3B(_channelInfo.color));
	_serverLabel->setPosition(200, 670);
	addChild(_serverLabel);
}
