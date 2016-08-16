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
	static Scene* createScene(DEF::ChannelInfo info);
	static LobbyScene* create(DEF::ChannelInfo info);

	virtual bool init() override;

	void menuCloseCallback(cocos2d::Ref* pSender);

private:
	void initLayout();
public:
private:
	DEF::ChannelInfo _channelInfo;
	short		_currentPage = 1;
	Label*		_serverLabel;
	std::string	_playerName;
	short		_pokeNum = 1;
};