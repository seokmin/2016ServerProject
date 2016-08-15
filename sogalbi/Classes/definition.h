#pragma once

namespace DEF
{
	enum SCREEN_SIZE
	{
		WIDTH = 1280,
		HEIGHT = 720
	};
	enum Z_ORDER
	{
		BACK = 0,
		CARD = 1,
		UI = 2
	};
	
	struct ChannelInfo
	{
		std::string name;
		std::string address;
		int port;
		cocos2d::Color3B color;
		int maxUser;
		int currentUser;
	};
}