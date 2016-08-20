#pragma once

#include "json/json.h"

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

	void leaveButtonClicked(Ref* pSender);

	void loginButtonClicked(Ref* pSender);
	void loginResponseArrived(network::HttpClient* sender, network::HttpResponse* response);

	void logoutButtonClicked(Ref* pSender);
	void logoutResponseArrived(network::HttpClient* sender, network::HttpResponse* response);


	void channelButtonClicked(DEF::ChannelInfo& clickChannel);

	CREATE_FUNC(LoginScene);

private:
	enum Z_ORDER : short
	{
		ERR = -1,
		BACKGROUND = 0,
		UI_LOGIN = 1,
		UI_CHANNELS_BG = 2,
		UI_CHANNELS_ABOVE = 3,
		UI_ALWAYS_TOP = 10
	};
	void initLayout();

	void popUpChannelsLayer(std::vector<DEF::ChannelInfo>& channelInfos);
	void popDownChannelsLayer();


	int parseChannelInfo(std::string& resLoginString, std::vector<DEF::ChannelInfo>& channelsVector);

	void connectChannel(std::string ip, int port);

	Json::Reader	_reader;

	Menu*			_loginMenu;
	Menu*			_leaveMenu;
	ui::TextField*	_nameField;
	ui::TextField*	_passField;

	Menu*			_logoutMenu;
	Menu*			_channelsMenu;
	Sprite*			_channelsBg;
	Label*			_channelsLabel;

	int				_currentChip = 0;
	std::string		_authToken;
	int				_pokeNum;
};