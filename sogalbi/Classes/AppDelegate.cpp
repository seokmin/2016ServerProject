#include "pch.h"
#include "AppDelegate.h"
#include "LoginScene.h"
#include "SimpleAudioEngine.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(DEF::SCREEN_SIZE::WIDTH, DEF::SCREEN_SIZE::HEIGHT);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("sogalbi", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("sogalbi");
#endif
        director->setOpenGLView(glview);
    }

#ifdef _DEBUG
	// 디버그시 디버깅 스탯 표시
	director->setDisplayStats(true);
#endif
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);

    register_all_packages();

	// 아틀라스 읽어오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures.plist");

    // create a scene. it's an autorelease object
    auto scene = LoginScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// 전화가 오는 등, 앱이 백그라운드로 들어갔을 때의 동작
void AppDelegate::applicationDidEnterBackground()
{

    //Director::getInstance()->stopAnimation();

	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// 앱이 재활성화 되었을 때의 동작
void AppDelegate::applicationWillEnterForeground()
{

    Director::getInstance()->startAnimation();

	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}