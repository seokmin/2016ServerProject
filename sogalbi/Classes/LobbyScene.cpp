#include "pch.h"
#include "LobbyScene.h"
#include "ResourceInfo.h"
#include "ui\UITextField.h"
#include "network\HttpRequest.h"
#include "network\HttpClient.h"

#pragma execution_character_set("UTF-8")

cocos2d::Scene* LobbyScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LobbyScene::create();
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
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}
void LobbyScene::initLayout()
{
	// 배경
	auto bg = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::LOBBY_BG);
	bg->setAnchorPoint(Vec2(0,0));
	bg->getTexture()->setAliasTexParameters();
	addChild(bg);

	// 나가기 버튼
	auto leaveLabel = Label::createWithTTF("나가기", FILENAME::FONT::SOYANON, 40);
	leaveLabel->setColor(Color3B(201, 201, 201));
	auto leaveButton = MenuItemLabel::create(leaveLabel, CC_CALLBACK_1(LobbyScene::menuCloseCallback, this));
	auto leaveMenu = Menu::create(leaveButton, nullptr);
	leaveMenu->setPosition(1199.f, 674.f);
	this->addChild(leaveMenu, DEF::Z_ORDER::UI);
}
