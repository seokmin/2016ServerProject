#include "pch.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ResourceInfo.h"
#include "Player.h"


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

	initLayout();
    
	// 네트워크 작업 스레딩
	auto scheduler = Director::getInstance()->getScheduler();
	//scheduler->performFunctionInCocosThread(std::bind());
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


void GameScene::initLayout()
{
	// 배경화면
	auto background = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::TABLE_BG);
	background->setAnchorPoint(Vec2(0, 0));
	background->getTexture()->setAliasTexParameters();
	this->addChild(background);

	// 나가기 버튼
	auto leaveLabel = Label::createWithTTF("나가기", FILENAME::FONT::SOYANON, 40);
	leaveLabel->setColor(Color3B(201, 201, 201));
	auto leaveButton = MenuItemLabel::create(leaveLabel, CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	auto leaveMenu = Menu::create(leaveButton, nullptr);
	leaveMenu->setPosition(1199.f, 674.f);
	this->addChild(leaveMenu);

	// 딜러 덱
	auto deck = Sprite::createWithSpriteFrameName(FILENAME::SPRITE::DECK);
	deck->setPosition(900.f, 700.f);
	deck->setRotation(-75.f);
	this->addChild(deck);

	// 방 번호
	auto roomText = std::string("Room No : ") + std::to_string(RandomHelper::random_int(1, 99));
	auto roomNum = Label::createWithTTF(roomText,FILENAME::FONT::SOYANON,32);
	roomNum->setColor(Color3B(255,255,0));
	roomNum->setPosition(20.f, 660.f);
	roomNum->setAnchorPoint(Vec2(0.f,0.f));
	this->addChild(roomNum);

	// 플레이어 네임택 와꾸
	for (auto& player : _players)
	{
		player = Player::create();
		addChild(player);
	}
	auto screenSize = getContentSize();
	_players[0]->setPosition(130, 260);
 	_players[1]->setPosition((screenSize.width/2+130)/2,190);
 	_players[2]->setPosition(screenSize.width/2,120);
 	_players[3]->setPosition(screenSize.width-_players[1]->getPosition().x,190);
	_players[4]->setPosition(screenSize.width-130,260);


}