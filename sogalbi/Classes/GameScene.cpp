#include "pch.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ResourceInfo.h"

#pragma execution_character_set("UTF-8")

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	InitLayout();
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
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


void GameScene::InitLayout()
{
	// 배경화면
	auto background = Sprite::create(FILENAME::SPRITE::TABLE_BG);
	background->setAnchorPoint(Vec2(0, 0));
	background->getTexture()->setAliasTexParameters();
	this->addChild(background, DEF::Z_ORDER::BACK);

	// 나가기 버튼
	auto leaveLabel = Label::createWithTTF("나가기", FILENAME::FONT::SOYANON, 40);
	leaveLabel->setColor(Color3B(201, 201, 201));
	auto leaveButton = MenuItemLabel::create(leaveLabel, CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	auto leaveMenu = Menu::create(leaveButton, nullptr);
	leaveMenu->setPosition(1199.f, 674.f);
	this->addChild(leaveMenu, DEF::Z_ORDER::UI);

	// 딜러 덱
	auto deck = Sprite::create(FILENAME::SPRITE::DECK);
	deck->setPosition(900.f, 700.f);
	deck->setRotation(-75.f);
	this->addChild(deck);

	auto roomText = std::string("Room No : ") + std::to_string(RandomHelper::random_int(1, 99));
	auto roomNum = Label::createWithTTF(roomText,FILENAME::FONT::SOYANON,32);
	roomNum->setColor(Color3B(255,255,0));
	roomNum->setPosition(20.f, 660.f);
	roomNum->setAnchorPoint(Vec2(0.f,0.f));
	this->addChild(roomNum);
}