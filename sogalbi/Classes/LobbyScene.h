#pragma once

namespace cocos2d
{
	namespace ui
	{
		class TextField;
	}
	namespace network
	{
		class HttpClient;
		class HttpResponse;
	}
}

class LobbyScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init() override;

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(LobbyScene);

private:
	void initLayout();
};