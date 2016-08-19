#pragma once

namespace DEF
{
	enum SCREEN_SIZE
	{
		WIDTH = 1280,
		HEIGHT = 720
	};
	struct ChannelInfo
	{
		std::string name;
		cocos2d::Color3B rgb;
		std::string ip;
		int port;
		int minBet;
		int maxBet;
	};
}