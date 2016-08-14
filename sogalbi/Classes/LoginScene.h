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

class LoginScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init() override;

	void menuCloseCallback(cocos2d::Ref* pSender);

	void menuStartCallback(cocos2d::Ref* pSender);

	void startResponseCallback(network::HttpClient* sender,network::HttpResponse* response);

	CREATE_FUNC(LoginScene);

private:
	void initLayout();
	ui::TextField* _nameField;
	ui::TextField* _passField;
};