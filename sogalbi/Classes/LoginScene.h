#pragma once

class LoginScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init() override;

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(LoginScene);

private:
	void initLayout();
};